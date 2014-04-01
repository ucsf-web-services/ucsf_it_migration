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
    parent::__construct($arguments);
  }
}
