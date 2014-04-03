<?php

/**
 * @file it_how_do.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT How Do" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItHowDoNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map image and attachment fields
    $this->addFieldMapping('field_it_how_to_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_how_to_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_how_to_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_how_to_images:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_it_how_to_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_how_to_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_how_to_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_how_to_attachments:language')->defaultValue(LANGUAGE_NONE);

    // map taxonomy term refs
    $this->addFieldMapping('field_it_how_to_taxonomy', '2')->sourceMigration('D6ItsCategoriesVocabulary');
    $this->addFieldMapping('field_it_how_to_taxonomy:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_it_how_to_taxonomy:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_it_how_to_taxonomy:ignore_case')->defaultValue(FALSE);

    // map related nodes
    $this->addFieldMapping('field_it_how_to_related_page', 'ref_nids');

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list', 'field_related_page', 'related_nids'));
    $this->addUnmigratedDestinations(array('field_it_how_to_images:alt', 'field_it_how_to_images:title',
      'field_it_how_to_attachments:description', 'field_it_how_to_attachments:display'));

  }

  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  public function query() {
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
