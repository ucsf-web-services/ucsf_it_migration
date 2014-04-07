<?php

/**
 * @file it_project.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Project" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItProjectNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map related pages
    $this->addFieldMapping('field_it_project_related_page', 'ref_nids');

    // map related services
    $this->addFieldMapping('field_it_project_related_service', 'related_service_nids')->separator(',')
      ->sourceMigration('D6ServiceNode');

    // map images and attachments
    $this->addFieldMapping('field_it_project_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_project_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_project_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_project_images:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_it_project_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_project_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_project_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_project_attachments:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_it_project_home_page_image', 'homepage_image_fid')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_project_home_page_image:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_project_home_page_image:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_project_home_page_image:language')->defaultValue(LANGUAGE_NONE);

    // map taxonomy term refs
    $this->addFieldMapping('field_it_project_taxonomy', 2)->sourceMigration('D6ItsCategoriesVocabulary');
    $this->addFieldMapping('field_it_project_taxonomy:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_it_project_taxonomy:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_it_project_taxonomy:ignore_case')->defaultValue(FALSE);

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list', 'field_related_page', 'related_nids', 'field_service',
      'field_related_page', 'field_home_page_image', 'field_home_page_image:list'));
    $this->addUnmigratedDestinations(array('field_it_project_images:alt', 'field_it_project_images:title',
      'field_it_project_home_page_image:alt', 'field_it_project_home_page_image:title',
      'field_it_project_attachments:description', 'field_it_project_attachments:display'));
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
    $query->leftJoin('content_type_it_project', 'ctip', 'ctip.vid = n.vid');
    $query->addField('ctip', 'field_home_page_image_fid', 'homepage_image_fid');

    // related services
    $query->leftJoin('content_field_service', 'cfs', 'cfs.vid = n.vid');
    $query->leftJoin('node', 'rsn', 'rsn.nid = cfs.field_service_nid');
    $query->addExpression('GROUP_CONCAT(DISTINCT rsn.nid)', 'related_service_nids');

    // get nids for related nodes.
    $query = $this->addRelatedNodesToQuery($query);

    $query->groupBy('n.vid');

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
