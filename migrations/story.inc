<?php
/**
 * @file story.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "Story" nodes from D6 to "Article" nodes in D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItStoryNode6Migration extends UcsfItNode6Migration
{
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments)
  {
    parent::__construct($arguments);
  }
}
