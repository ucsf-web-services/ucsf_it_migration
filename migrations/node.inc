<?php

/**
 * @file node.inc
 */

/**
 * Node Migrations base class for the UCSF IT website migration.
 *
 * @see DrupalNode6Migration
 */
abstract class UcsfItNode6Migration extends DrupalNode6Migration
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

    $this->addUnmigratedDestinations(array('field_tags', 'field_tags:source_type', 'field_tags:create_term', 'field_tags:ignore_case'));

    $this->addUnmigratedSources(array('revision', 'revision_uid', 'log'));
  }

  /**
   * @return SelectQueryInterface
   *
   * @see DrupalNodeMigration::query()
   */
  protected function query()
  {
    $query = parent::query();
    // @todo Augment query to pull in columns from  statistics and pathauto tables. [ST 2014/03/24]
    return $query;
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
