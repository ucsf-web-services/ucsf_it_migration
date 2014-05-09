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
    // Kinda inefficient, but "good enough for government work".
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

