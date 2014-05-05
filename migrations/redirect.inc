<?php

/**
 * @file
 *
 * Migrates redirects from Drupal6 ("path_redirect" module) into Drupal7 ("redirect" module).
 */
class UcsfItRedirectMigration extends DrupalMigration {

  /**
   * @param array $arguments
   */
  public function __construct($arguments) {


    $this->sourceFields = array(
      'rid' => t('Redirect ID'),
/*
      'source' => t('The source path'),
      'redirect' => t('The redirect destination path'),
      'query' => t('Query parameters'),
      'fragment' => t('Hash tag'),
      'language' => t('Language key'),
      'type' => t('HTTP response code'),
      'last_used' => t('Last used timestamp'),
*/
    );

    parent::__construct($arguments);

    // Create our three main objects - source, destination, and map
    $this->setSource(new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions));

    $this->setDestination(new UcsfItRedirectMigrationDestination());

    $this->setMap(new MigrateSQLMap($this->machineName,
      array(
        'rid' => array('type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source redirect',
          'alias' => 'pr',
        ),
      ),
      UcsfItRedirectMigrationDestination::getKeySchema()
    ));
  }

  /**
   * The base source query for this migration.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('path_redirect', 'pr')
      ->fields('pr', array('rid', 'source', 'redirect', 'query', 'fragment', 'language', 'type', 'last_used'));
    return $query;
  }
}

/**
 * Migration Destination for path redirects.
 */
class UcsfItRedirectMigrationDestination extends MigrateDestination {

  /**
   * @return array
   */
  static public function getKeySchema() {
    return array(
      'rid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination redirect',
      ),
    );
  }

  /**
   *
   * Returns the description of the migration destination.
   *
   * @return string
   *  Description of the destination being migrated into.
   */
  public function __toString() {
    return 'Redirect';
  }

  /**
   * Returns a list of available destination fields.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {

    $fields = array('rid', 'hash', 'uid', 'source', 'source_options', 'redirect', 'redirect_options', 'language', 'status_code', 'count', 'access');
    return $fields;

  }

  /**
   * @override
   * @see MigrateDestination::import()
   * @param stdClass $object
   * @param stdClass $row
   * @return array the id of the migrated redirect record.
   *
   * @todo Adjust internal node paths. [ST 2014/05/05]
   */
  public function import(stdClass $object, stdClass $row) {

    // hardwired. good enough for now!
    $redirect_defaults = array(
      'status_code' => 301,
      'uid' => 1,
      'language' => LANGUAGE_NONE,
    );

    $redirect = new stdClass();
    redirect_object_prepare($redirect, $redirect_defaults);
    $redirect->redirect = $row->redirect;
    $redirect->source = $row->source;

    // save the redirect.
    redirect_save($redirect);

    // return the id of the newly created redirect record.
    return array($redirect->rid);
  }

  /**
   * Rolls back redirect migration.
   * @param array $rids A list of ids belonging to migrated redirects.
   * @see redirect_delete_multiple()
   */
  public function bulkRollback(array $rids) {
    migrate_instrument_start('redirect_delete_multiple');
    redirect_delete_multiple($rids);
    migrate_instrument_stop('redirect_delete_multiple');
  }
}

