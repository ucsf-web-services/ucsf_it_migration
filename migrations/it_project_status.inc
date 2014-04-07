<?php

/**
 * @file it_project_status.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Project Status" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItProjectStatusNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // map related project
    $this->addFieldMapping('field_it_project_status_rel_proj', 'field_related_project')
      ->sourceMigration('D6ProjectNode');

    // map status field
    $this->addFieldMapping('field_it_project_status_status', 'field_project_status');

    // DNM
    $this->addUnmigratedSources(array('field_project_status:format'));
  }
}
