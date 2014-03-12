<?php

/**
 * Implements hook_migrate_api().
 */
function migrate_xyz_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}
