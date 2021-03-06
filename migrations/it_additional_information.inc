<?php

/**
 * @file it_additional_information.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Service Additional Information" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItServiceAdditionalInfoNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // map image and attachment fields
    $this->addFieldMapping('field_it_additional_info_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_additional_info_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_additional_info_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_additional_info_images:language')->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('field_it_additional_info_attchmt', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_additional_info_attchmt:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_additional_info_attchmt:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_additional_info_attchmt:language')->defaultValue(LANGUAGE_NONE);

    // node ref to service
    $this->addFieldMapping('field_it_additional_info_service', 'field_additional_service_info')->sourceMigration('D6ServiceNode');

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list'));
    $this->addUnmigratedDestinations(array('field_it_additional_info_attchmt:description', 'field_it_additional_info_attchmt:display',
      'field_it_additional_info_images:alt', 'field_it_additional_info_images:title'));
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
    return $query;
  }
}
