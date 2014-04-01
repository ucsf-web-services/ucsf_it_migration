<?php

/**
 * @file it_outage.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Status" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItStatusNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}
