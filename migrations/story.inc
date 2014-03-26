<?php
/**
 * @file story.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "Story" nodes from D6 to "Article" nodes in D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItStoryNode6Migration extends UcsfItNode6Migration
{
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments)
  {
    parent::__construct($arguments);

    $this->addUnmigratedDestinations(array('field_tags', 'field_tags:source_type', 'field_tags:create_term',
      'field_tags:ignore_case', 'field_image', 'field_image:file_class', 'field_image:language', 'field_image:preserve_files',
      'field_image:destination_dir', 'field_image:destination_file', 'field_image:file_replace', 'field_image:source_dir',
      'field_image:urlencode', 'field_image:alt', 'field_image:title'));

    $this->addUnmigratedSources(array('revision', 'revision_uid', 'log', 'upload', 'upload:description', 'upload:list',
      'upload:weight'));
  }

  /**
   * @return SelectQueryInterface
   *
   * @see DrupalNodeMigration::query()
   */
  protected function query()
  {
    $query = parent::query();
    // add node stats
    $query->leftJoin('node_counter', 'nc', 'n.nid = nc.nid');
    $query->addField('nc', 'timestamp');
    $query->addField('nc', 'daycount');
    $query->addField('nc', 'totalcount');

    return $query;
  }

  /**
   * @param array $row
   * @return boolean
   *
   * @see DrupalNodeMigration::prepareRow()
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    // @todo Implement. [ST 2014/03/24]
  }
}