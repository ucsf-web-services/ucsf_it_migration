<?php

/**
 * @file it_news.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT News" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItNewsNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // @todo implement [ST 03/31/2014]
  }

  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  public function query() {
    $query = parent::query();
    // @todo implement [ST 03/31/2014]
    return $query;
  }
}
