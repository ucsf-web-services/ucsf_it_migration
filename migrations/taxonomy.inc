<?php
/**
 * @file taxonomy.inc
 */

/**
 * Taxonomy migration class for Drupal 6.
 * Handles synonyms migration.
 *
 * @see DrupalTerm6Migration
 */
class UcsfItTerm6Migration extends DrupalTerm6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->team = array(
      new MigrateTeamMember('Stefan Topfstedt', 'stefan.topfstedt@ucsf.edu', t('Migration Implementer')),
    );

    $this->addFieldMapping('synonyms_synonyms', 'synonyms')
      ->description(t('Preserve synonyms during taxonomy term migration.'))
      ->separator(';');

    $this->addFieldMapping('synonyms_synonyms:language')
      ->defaultValue('und');
  }


  /**
   * @see DrupalTerm6Migration::query()
   * @return SelectQueryInterface
   */
  protected function query() {
    $query = parent::query();
    $query->leftJoin("term_synonym", 'ts', 'ts.tid = td.tid');
    $query->addExpression('GROUP_CONCAT(DISTINCT ts.name SEPARATOR \';\')', 'synonyms');
    $query->groupBy('td.tid');
    return $query;
  }
}
