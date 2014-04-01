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
    parent::__construct($arguments);
  }
}
