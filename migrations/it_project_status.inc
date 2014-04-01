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
  }
}
