<?php

/**
 * @file it_team.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Team" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItTeamNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map parent unit
    $this->addFieldMapping('field_it_team_parent_unit', 'field_parent_unit');

    // map manager field
    $this->addFieldMapping('field_it_team_team_manager', 'field_manager');
    $this->addFieldMapping('field_it_team_team_manager:language')->defaultValue(LANGUAGE_NONE);

    // map related pages
    $this->addFieldMapping('field_it_team_related_page', 'ref_nids');

    // map related projects
    $this->addFieldMapping('field_it_team_projects', 'related_project_nids')->separator(',')
      ->sourceMigration('D6ProjectNode');

    // map related services
    $this->addFieldMapping('field_it_team_services', 'related_service_nids')->separator(',')
      ->sourceMigration('D6ServiceNode');

    // map images and attachments
    $this->addFieldMapping('field_it_team_images', 'image_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_team_images:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_team_images:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_team_images:language')->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping('field_it_team_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_team_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_team_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_team_attachments:language')->defaultValue(LANGUAGE_NONE);

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_image',
      'field_image:list', 'field_related_page', 'related_nids', 'field_team_projects', 'field_services',
      'field_related_page'));
    $this->addUnmigratedDestinations(array('field_it_team_images:alt', 'field_it_team_images:title',
      'field_it_team_attachments:description', 'field_it_team_attachments:display'));
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

    // related services
    $query->leftJoin('content_field_services', 'cfs', 'cfs.vid = n.vid');
    $query->leftJoin('node', 'rsn', 'rsn.nid = cfs.field_services_nid');
    $query->addExpression('GROUP_CONCAT(DISTINCT rsn.nid)', 'related_service_nids');

    // related projects
    $query->leftJoin('content_field_team_projects', 'cftp', 'cftp.vid = n.vid');
    $query->leftJoin('node', 'rpn', 'rpn.nid = cftp.field_team_projects_nid');
    $query->addExpression('GROUP_CONCAT(DISTINCT rpn.nid)', 'related_project_nids');

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
