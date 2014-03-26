<?php
/**
 * @file page.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "Page" nodes from D6 to "Basic Page" nodes in D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItPageNode6Migration extends UcsfItNode6Migration
{
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments)
  {
    parent::__construct($arguments);

    $this->addUnmigratedDestinations(array());
  }

  /**
   * @param array $row
   * @return boolean
   *
   * @see DrupalNodeMigration::prepareRow()
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    // @todo Implement. [ST 2014/03/24]
  }
}
