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

    // map display title
    $this->addFieldMapping('field_it_status_display_title', 'field_status_display_title');
    $this->addFieldMapping('field_it_status_display_title:language')->defaultValue('und');

    // map taxonomy term refs
    $this->addFieldMapping('field_it_status_taxonomy', '2')->sourceMigration('D6ItsCategoriesVocabulary');
    $this->addFieldMapping('field_it_status_taxonomy:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_it_status_taxonomy:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_it_status_taxonomy:ignore_case')->defaultValue(FALSE);

    // reason/impact/resolution
    $this->addFieldMapping('field_it_status_reason', 'field_reason');
    $this->addFieldMapping('field_it_status_reason:language')->defaultValue('und');
    $this->addFieldMapping('field_it_status_reason:format')->defaultValue('full_html');
    $this->addFieldMapping('field_it_status_impact', 'field_impact');
    $this->addFieldMapping('field_it_status_impact:language')->defaultValue('und');
    $this->addFieldMapping('field_it_status_impact:format')->defaultValue('full_html');
    $this->addFieldMapping('field_it_status_resolution', 'field_status_resolution');
    $this->addFieldMapping('field_it_status_resolution:language')->defaultValue('und');
    $this->addFieldMapping('field_it_status_resolution:format')->defaultValue('full_html');

    // map private flag
    $this->addFieldMapping('field_it_status_private', 'field_private');

    // map status type
    $this->addFieldMapping('field_it_status_status_type', 'field_status_type');

    // map outage date
    $this->addFieldMapping('field_it_status_outage_date', 'field_outage_date'); // start date
    $this->addFieldMapping('field_it_status_outage_date:rrule', 'field_outage_date:rrule');
    $this->addFieldMapping('field_it_status_outage_date:to', 'field_outage_date:value2'); // end date
    $this->addFieldMapping('field_it_status_outage_date:timezone')->defaultValue('America/Los_Angeles'); // we are HERE!
  }

  /**
   * @param $row
   * @return bool
   *
   * @see DrupalNodeMigration::prepareRow()
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // data massaging:
    // convert text to integer
    if ('Public' ===  $row->field_private) {
      $row->field_private = 0;
    } else {
      $row->field_private = 1;
    }

    // convert absolute to relative urls
    $row->field_it_status_reason = $this->convertAbsoluteToRelativeUrls($row->field_it_status_reason);
    $row->field_it_status_impact = $this->convertAbsoluteToRelativeUrls($row->field_it_status_impact);
    $row->field_it_status_resolution = $this->convertAbsoluteToRelativeUrls($row->field_it_status_resolution);
  }
}
