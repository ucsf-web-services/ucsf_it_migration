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
    parent::__construct($arguments);
  }
}
