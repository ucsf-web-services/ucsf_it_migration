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

    // map policy type
    $this->addFieldMapping('field_policy_or_standard_type', 'field_policy_type');

    // map taxonomy term refs
    $this->addFieldMapping('field_policy_or_standard_tax', '2')->sourceMigration('D6ItsCategoriesVocabulary');
    $this->addFieldMapping('field_policy_or_standard_tax:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_policy_or_standard_tax:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_policy_or_standard_tax:ignore_case')->defaultValue(FALSE);

    // map file attachment refs
    $this->addFieldMapping('field_policy_or_standard_attach', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_policy_or_standard_attach:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_policy_or_standard_attach:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_policy_or_standard_attach:language')->defaultValue(LANGUAGE_NONE);

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list'));

    $this->addUnmigratedDestinations(array('field_policy_or_standard_attach:description',
      'field_policy_or_standard_attach:display'));
  }

  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  protected function query() {
    $query = parent::query();
    // get the file-ids for associated attachment-files here
    $query->leftJoin('content_field_attached_file', 'cfa', 'cfa.vid = n.vid');
    $query->addExpression('GROUP_CONCAT(DISTINCT cfa.field_attached_file_fid)', 'attachment_fids');
    $query->groupBy('n.vid');
    return $query;
  }
}
