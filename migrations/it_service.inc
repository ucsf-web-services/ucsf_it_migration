<?php

/**
 * @file it_service.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Service" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItServiceNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map service title
    $this->addFieldMapping('field_it_service_service_title', 'field_service_title');
    $this->addFieldMapping('field_it_service_service_title:language')->defaultValue(LANGUAGE_NONE);

    // map metadata
    $this->addFieldMapping('field_it_service_meta_data', 'field_meta_data');
    $this->addFieldMapping('field_it_service_meta_data:language')->defaultValue(LANGUAGE_NONE);

    // map "service now" link
    $this->addFieldMapping('field_it_service_place_order', 'field_service_now');
    $this->addFieldMapping('field_it_service_place_order:title', 'field_service_now:title');
    $this->addFieldMapping('field_it_service_place_order:attributes', 'field_service_now:attributes');
    $this->addFieldMapping('field_it_service_place_order:language')->defaultValue(LANGUAGE_NONE);

    // map service short description
    $this->addFieldMapping('field_it_service_short_descr', 'field_service_description');
    $this->addFieldMapping('field_it_service_short_descr:language')->defaultValue(LANGUAGE_NONE);

    // map audience list
    $this->addFieldMapping('field_it_service_audience', 'field_audience');

    // map service provider list
    $this->addFieldMapping('field_it_service_srvc_provider', 'field_provider');

    // map images and attachments
    $this->addFieldMapping('field_it_service_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_service_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_service_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_service_images:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_it_service_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_service_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_service_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_service_attachments:language')->defaultValue(LANGUAGE_NONE);

    // map taxonomy term refs
    $this->addFieldMapping('field_it_service_taxonomy', 2)->sourceMigration('D6ItsCategoriesVocabulary');
    $this->addFieldMapping('field_it_service_taxonomy:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_it_service_taxonomy:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_it_service_taxonomy:ignore_case')->defaultValue(FALSE);


    // map node refs
    $this->addFieldMapping('field_it_service_related_page', 'ref_nids');

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list', 'field_related_page', 'related_nids'));
    $this->addUnmigratedDestinations(array('field_it_service_images:alt', 'field_it_service_images:title',
      'field_it_service_attachments:description', 'field_it_service_attachments:display'));
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
    $row->mapped_tids = array('3', '6', '8', '9');
  }
}
