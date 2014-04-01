<?php

/**
 * @file it_category_description.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Category Description" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItCategoryDescriptionNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}
