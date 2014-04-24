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
    parent::__construct($arguments);

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

    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new UcsfItRedirectMigrationDestination();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'rid' => array('type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source redirect',
          'alias' => 'pr',
        ),
      ),
      UcsfItRedirectMigrationDestination::getKeySchema()
    );
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
   */
  public function import(stdClass $object, stdClass $row) {

    // @todo implement. [ST 2014/04/23]
  }
}
