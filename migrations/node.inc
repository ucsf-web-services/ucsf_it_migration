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

    $this->setIssuePattern('https://bitbucket.org/ucsfdrupal/ucsf_it_migration/issue/:id:');

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
   * Returns the source migration mapped to a given content type.
   * @param string $content_type The content type's machine name.
   * @return string|boolean The source migration name, or FALSE if none was found.
   *
   * @todo Map out any remaining content types [ST 2014/03/31].
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
      default :
        // do nothing
    }
    return $migration;
  }
}
