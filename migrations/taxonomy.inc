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
class UcsfItTerm6Migration extends DrupalTerm6Migration
{
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments)
  {
    parent::__construct($arguments);

    $this->team = array(
      new MigrateTeamMember('Stefan Topfstedt', 'stefan.topfstedt@ucsf.edu', t('Migration Implementer')),
    );


    $this->setIssuePattern('https://bitbucket.org/ucsfdrupal/ucsf_it_migration/issue/:id:');

    $this->addFieldMapping('synonyms_synonyms', 'synonyms')
      ->description(t('Preserve synonyms during taxonomy term migration.'))
      // ->issueGroup('Customizations')
      ->issueNumber('1')
      ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_OK)
      ->separator(';');

    $this->addFieldMapping('synonyms_synonyms:language')
      ->defaultValue('und');
  }


  /**
   * @see DrupalTerm6Migration::query()
   * @return SelectQueryInterface
   */
  protected function query()
  {
    $query = parent::query();
    $query->leftJoin("term_synonym", 'ts', 'ts.tid = td.tid');
    $query->addExpression('GROUP_CONCAT(DISTINCT ts.name SEPARATOR \';\')', 'synonyms');
    $query->groupBy('td.tid');
    return $query;
  }
}
