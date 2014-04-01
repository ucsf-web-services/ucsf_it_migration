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
  }
}
