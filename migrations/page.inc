<?php
/**
 * @file page.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "Page" nodes from D6 to "Basic Page" nodes in D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItPageNode6Migration extends UcsfItNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map image and attachment fields
    // @link https://drupal.org/node/1819704
    // @link http://drupal.stackexchange.com/a/58637/1801
    $this->addFieldMapping('field_page_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_page_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_page_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_page_images:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_page_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_page_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_page_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_page_attachments:language')->defaultValue(LANGUAGE_NONE);

    // map related nodes
    $this->addFieldMapping('field_page_related_page', 'ref_nids');

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list', 'field_related_page', 'related_nids'));
    $this->addUnmigratedDestinations(array('field_page_images:alt', 'field_page_images:title',
      'field_page_attachments:description', 'field_page_attachments:display'));
  }


  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  protected function query() {
    $query = parent::query();
    // get the file-ids for associated images and attachments here
    $query->leftJoin('content_field_image', 'cfi', 'cfi.vid = n.vid');
    $query->addExpression('GROUP_CONCAT(DISTINCT cfi.field_image_fid)', 'image_fids');
    $query->leftJoin('content_field_attached_file', 'cfa', 'cfa.vid = n.vid');
    $query->addExpression('GROUP_CONCAT(DISTINCT cfa.field_attached_file_fid)', 'attachment_fids');
    $query->groupBy('n.vid');
    // get nids for related nodes.
    $query = $this->addRelatedNodesToQuery($query);
    return $query;
  }

  /**
   * @param $row
   * @return bool
   *
   * @see DrupalNodeMigration::prepareRow()
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $row->ref_nids = $this->processRelatedNids($row->related_nids);
  }
}
