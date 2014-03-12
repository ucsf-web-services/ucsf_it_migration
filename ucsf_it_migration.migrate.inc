<?php

/**
 * Implements hook_migrate_api().
 */
function ucsf_it_migration_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}
