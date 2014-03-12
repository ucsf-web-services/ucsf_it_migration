<?php

/**
 * Implements hook_migrate_api().
 */
function migrate_xyz_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'ucsf_it' => array(
        'title' => t('UCSF IT Migrations'),
      ),
    ),
    'migrations' => array(
      'UcsfItUser' => array(
        'class_name' => 'MigrateUcsfItUserMigration',
        'group_name' => 'ucsf_it',
      ),
    ),
  );
  return $api;
}
