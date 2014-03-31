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
    parent::__construct($arguments);
  }
}
