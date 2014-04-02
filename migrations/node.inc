<?php

/**
 * @file node.inc
 */

/**
 * Node Migrations base class for the UCSF IT website migration.
 *
 * @see DrupalNode6Migration
 */
abstract class UcsfItNode6Migration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->team = array(
      new MigrateTeamMember('Stefan Topfstedt', 'stefan.topfstedt@ucsf.edu', t('Migration Implementer')),
    );

    // always list revision info and whatever "upload" is as "do not migrate" source fields.
    $this->addUnmigratedSources(array('revision', 'revision_uid', 'log', 'upload', 'upload:description', 'upload:list',
      'upload:weight'));
  }

  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  protected function query() {
    $query = parent::query();
    // add node stats
    $query->leftJoin('node_counter', 'nc', 'n.nid = nc.nid');
    $query->addField('nc', 'timestamp');
    $query->addField('nc', 'daycount');
    $query->addField('nc', 'totalcount');

    return $query;
  }

  /**
   * Retrieves the content type of a given node.
   *
   * @param int $nid The node id.
   * @return string|boolean The machine name of the node's content type, or FALSE if none was found.
   */
  protected function getContentTypeByNid($nid) {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('node', 'n')
      ->fields('n', array('type'))
      ->condition('n.nid', $nid);
    $result = $query->execute();
    if (empty($result)) {
      return FALSE;
    }
    $row = $result->fetchAssoc();
    return $row['type'];
  }

  /**
   * Returns the source migration mapped to a given legacy (D6) content type.
   * @param string $content_type The legacy content type's machine name.
   * @return string|boolean The source migration name, or FALSE if none was found.
   */
  protected function getSourceMigrationByContentType($content_type) {
    $migration = FALSE;
    switch ($content_type) {
      case 'page' :
        $migration = 'D6PageNode';
        break;
      case 'story' :
        $migration = 'D6StoryNode';
        break;
      case 'it_news' :
        $migration = 'D6NewsNode';
        break;
      case 'it_how_to' :
        $migration = 'D6HowDoNode';
        break;
      case 'it_outage' :
        $migration = 'D6StatusNode';
        break;
      case 'it_service' :
        $migration = 'D6ServiceNode';
        break;
      case 'it_additional_information' :
        $migration = 'D6AdditionalInfoNode';
        break;
      case 'it_service_tutorial' :
        $migration = 'D6TutorialNode';
        break;
      case 'it_category_description' :
        $migration = 'D6CategoryDescriptionNode';
        break;
      case 'it_project' :
        $migration = 'D6ProjectNode';
        break;
      case 'it_project_status' :
        $migration = 'D6ProjectStatusNode';
        break;
      case 'it_team' :
        $migration = 'D6TeamNode';
        break;
      case 'policy_standard' :
        $migration = 'D6PolicyNode';
        break;
      default :
        // do nothing
    }
    return $migration;
  }

  /**
   * Helper method that maps source node references to target entity references.
   * Call this method from within your <code>prepareRow()</code> implementation.
   * Source node references are expected to be passed as an array in the <code>related_nids</code>
   * @param string $related_nids A comma-separated list of source node ids.
   * @return array An array of mapped target node ids.
   */
  protected function processRelatedNids($related_nids) {
    $ref_nids = array();
    // process related nids
    if ($related_nids) {
      $source_ids = explode(',', $related_nids);
      foreach ($source_ids as $source_id) {
        $content_type = $this->getContentTypeByNid($source_id); // figure out the content type of each related node
        // figure out the migration responsible for the identified legacy content type
        $migration = $this->getSourceMigrationByContentType($content_type);
        // handle it. this should return a node id in the migration destination.
        if ($migration) {
          $ref_nids[] = $this->handleSourceMigration($migration, $source_id);
        }
      }
    }
    return $ref_nids;
  }

  /**
   * Helper method that augments a given query to retrieve NIDs of related nodes.
   * Call this method from within your <code>query()</code> implementation.
   * @param SelectQueryInterface $query The query object.
   * @return SelectQueryInterface The augmented query object.
   */
  protected function addRelatedNodesToQuery(SelectQueryInterface $query) {
    $query->leftJoin('content_field_related_page', 'cfrp', 'cfrp.vid = n.vid');
    $query->leftJoin('node', 'rn', 'rn.nid = cfrp.field_related_page_nid');
    $query->addExpression('GROUP_CONCAT(DISTINCT rn.nid)', 'related_nids');
    return $query;
  }
}
