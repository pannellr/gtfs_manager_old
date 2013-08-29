<?php
/*
 * @file gtfs-manager-all-features
 * displays all existing features and their icons
 */
?>

<h2>All features</h2>

<?php foreach ($variables['service_features'] as $feature) {?>
<?php print $feature->markup; ?>
<?php } ?>