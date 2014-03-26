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
  }

  /**
   * @return SelectQueryInterface
   *
   * @see DrupalNodeMigration::query()
   */
  protected function query()
  {
    $query = parent::query();
    // add node stats
    $query->leftJoin('node_counter', 'nc', 'n.nid = nc.nid');
    $query->addField('nc', 'timestamp');
    $query->addField('nc', 'daycount');
    $query->addField('nc', 'totalcount');

    return $query;
  }
}
