<?php

/**
 * @file policy_or_standard.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT Policy or Standard" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItPolicyOrStandardNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}
