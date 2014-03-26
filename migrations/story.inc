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

    $this->addUnmigratedDestinations(array('field_tags', 'field_tags:source_type', 'field_tags:create_term',
      'field_tags:ignore_case', 'field_image', 'field_image:file_class', 'field_image:language', 'field_image:preserve_files',
      'field_image:destination_dir', 'field_image:destination_file', 'field_image:file_replace', 'field_image:source_dir',
      'field_image:urlencode', 'field_image:alt', 'field_image:title'));
  }
}
