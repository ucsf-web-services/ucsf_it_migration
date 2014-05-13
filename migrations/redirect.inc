<?php

/**
 * @file
 *
 * Migrates redirects from Drupal6 ("path_redirect" module) into Drupal7 ("redirect" module).
 */
class UcsfItRedirectMigration extends DrupalMigration {

  /**
   * @var array
   */
  protected $_node_migrations = array();

  /**
   * @param array $arguments
   */
  public function __construct($arguments) {

    $this->sourceFields = array(
      'rid' => t('Redirect ID'),
    );

    parent::__construct($arguments);

    // get node migrations for lookup.
    $this->_node_migrations[] = Migration::getInstance('D6StoryNode');
    $this->_node_migrations[] = Migration::getInstance('D6StatusNode');
    $this->_node_migrations[] = Migration::getInstance('D6PageNode');
    $this->_node_migrations[] = Migration::getInstance('D6NewsNode');
    $this->_node_migrations[] = Migration::getInstance('D6HowDoNode');
    $this->_node_migrations[] = Migration::getInstance('D6ServiceNode');
    $this->_node_migrations[] = Migration::getInstance('D6AdditionalInfoNode');
    $this->_node_migrations[] = Migration::getInstance('D6TutorialNode');
    $this->_node_migrations[] = Migration::getInstance('D6CategoryDescriptionNode');
    $this->_node_migrations[] = Migration::getInstance('D6ProjectNode');
    $this->_node_migrations[] = Migration::getInstance('D6ProjectStatusNode');
    $this->_node_migrations[] = Migration::getInstance('D6PolicyNode');
    $this->_node_migrations[] = Migration::getInstance('D6TeamNode');

    // Create our three main objects - source, destination, and map
    $this->setSource(new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions));

    $this->setDestination(new UcsfItRedirectMigrationDestination($this->_node_migrations));

    $this->setMap(new MigrateSQLMap($this->machineName,
      array(
        'rid' => array('type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source redirect',
          'alias' => 'pr',
        ),
      ),
      UcsfItRedirectMigrationDestination::getKeySchema()
    ));

    $this->addUnmigratedSources(array('source', 'redirect', 'query', 'fragment', 'language', 'type', 'last_used'));
  }

  /**
   * The base source query for this migration.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('path_redirect', 'pr')
      ->fields('pr', array('rid', 'source', 'redirect', 'query', 'fragment', 'language', 'type', 'last_used'));
    return $query;
  }

  /**
   * Runs after migration completes.
   *
   * We use it here to do one more pass over all migrated nodes and adjust internal node paths in the content itself.
   * This is terribly hacky, but since this migration is intended to run after all the node migrations have been
   * processed, it's not the worst of all places.
   *
   * @see Migration::postImport()
   */
  public function postImport() {
    parent::postImport();

    /*
    Plan of action:
    --------------
    1. get all node bodies/summaries
    2. loop through them, for each one
    2a.   find internal node paths and replace the source node-ids with their migrated counterparts
    2b.   save all node bodies back.
    */

    // 1.
    $node_contents = $this->_get_node_bodies();
    // 2.
    foreach ($node_contents as $nid => $content) {
      $body_hash = md5($content['body_value']);
      $summary_hash = md5($content['body_summary']);
      // 2a.
      $updated_body = $this->_fix_internal_node_paths($content['body_value']);
      $updated_summary = $this->_fix_internal_node_paths($content['body_summary']);
      $updated_body_hash = md5($updated_body);
      $updated_summary_hash = md5($updated_summary);
      if (strcmp($body_hash, $updated_body_hash) || strcmp($summary_hash, $updated_summary_hash)) { // has anything changed?
        // 2b.
        $this->_update_node_body($nid, $content['vid'], $updated_body, $updated_summary);
      }
    }
  }

  /**
   * Updates the body and summary for a given node revision.
   * @param int $nid The node id
   * @param int $vid The revision id.
   * @param string $body The node body.
   * @param string $summary The node summary.
   */
  private function _update_node_body($nid, $vid, $body, $summary) {
    $sql=<<< EOL
UPDATE {field_data_body}
SET body_value = %s, body_summary = %s
WHERE entity_id = %d AND revision_id = %d AND entity_type = 'node'
EOL;
    db_query($sql, array($body, $summary, $nid, $vid));
  }

  /**
   * Retrieves all node bodies/summaries.
   * @return array An associative array of arrays, keyed off by node id.
   *  Each item is an associative array itself, containing the following items:
   *    'vid'          ... The node's revision id.
   *    'body_value'   ... The node body.
   *    'body_summary' ... The body summary.
   */
  private function _get_node_bodies() {
    $rhett = array();
    $sql =<<< EOL
SELECT nr.nid, nr.vid, f.body_value, f.body_summary
FROM {node} n
LEFT JOIN {node_revision} nr ON n.vid = nr.vid
LEFT JOIN {field_data_body} f ON f.revision_id = nr.vid AND f.entity_id = nr.nid
WHERE f.entity_type = 'node'
EOL;

    $result = db_query($sql);
    foreach ($result as  $row) {
      $rhett[$row->nid] = array(
        'body_value' => $row->body_value,
        'body_summary' => $row->body_summary,
        'vid' => $row->vid,
      );
    }
    return $rhett;
  }

  /**
   * Replaces internal node paths from the migration source in a given markup text
   * with their equivalents in the migration destination.
   * @param string $text The markup to modify.
   * @return string The modified markup.
   */
  protected function _fix_internal_node_paths ($text) {
    // @todo Implement [ST 2014/05/12]
    return $text;
  }
}

/**
 * Migration Destination for path redirects.
 */
class UcsfItRedirectMigrationDestination extends MigrateDestination {

  /**
   * @var array
   */
  protected $_node_migrations;


  public function __construct(array $node_migrations) {

    $this->_node_migrations = $node_migrations;
  }

  /**
   * @return array
   */
  static public function getKeySchema() {
    return array(
      'rid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination redirect',
      ),
    );
  }

  /**
   *
   * Returns the description of the migration destination.
   *
   * @return string
   *  Description of the destination being migrated into.
   */
  public function __toString() {
    return 'Redirect';
  }

  /**
   * Returns a list of available destination fields.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {

    return array();

  }

  /**
   * @override
   * @see MigrateDestination::import()
   *
   * @param stdClass $object
   * @param stdClass $row
   * @return array the id of the migrated redirect record.
   * @throws MigrateException
   */
  public function import(stdClass $object, stdClass $row) {

    // hardwired. good enough for now!
    $redirect_defaults = array(
      'status_code' => 301,
      'uid' => 1,
      'language' => LANGUAGE_NONE,
    );

    $redirect = new stdClass();
    redirect_object_prepare($redirect, $redirect_defaults);
    $redirect->redirect = $row->redirect;
    $redirect->source = $row->source;
    $redirect->status_code = $row->type;

    // update internal node paths.
    // this assumes that all node migrations have already run.
    if (0 === strpos($redirect->redirect, 'node/')) {
      if (false === ($redirect->redirect = $this->_getMigratedNodePath($redirect->redirect))) {
        throw new MigrateException('Not found.', Migration::MESSAGE_NOTICE);
      }
    }
    if (0 === strpos($redirect->source, 'node/')) {
      if (false === ($redirect->redirect = $this->_getMigratedNodePath($redirect->source))) {
        throw new MigrateException('Not found.', Migration::MESSAGE_NOTICE);
      }
    }

    // save the redirect.
    redirect_save($redirect);

    // return the id of the newly created redirect record.
    return array($redirect->rid);
  }

  /**
   * Rolls back redirect migration.
   * @param array $rids A list of ids belonging to migrated redirects.
   * @see redirect_delete_multiple()
   */
  public function bulkRollback(array $rids) {
    migrate_instrument_start('redirect_delete_multiple');
    redirect_delete_multiple($rids);
    migrate_instrument_stop('redirect_delete_multiple');
  }

  /**
   * Retrieves the migrated internal path of a node for its given source path.
   * @param string $path The internal node path in the migration source.
   * @return string|boolean The node path in the migration destination, or FALSE if no path could be determined.
   */
  protected function _getMigratedNodePath($path) {
    $source_nid = (int) str_replace('node/', '', $path);
    $dest_nid = FALSE;
    // KLUDGE!
    // Iterate over node migrations and peel out the destination node id for each given node path.
    // Stop on first match.
    // [ST 2015/05/05]
    foreach ($this->_node_migrations as $node_migration) {
      $destids = $node_migration->getMap()->lookupDestinationID(array($source_nid));
      if (! empty($destids['destid1'])) {
        $dest_nid = $destids['destid1'];
        break;
      }
    }
    // return the internal path.
    return 'node/' . $dest_nid;
  }
}

