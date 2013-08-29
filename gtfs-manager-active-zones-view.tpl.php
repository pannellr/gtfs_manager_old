<?php
/*
 * @file gtfs-manager-active-zones
 *
 * displays routes by zone displayed in different colors
 */
?>

<?php foreach ($variables['zone_groups'] as $zone_id => $zone_group) {?>
  <div class="<?php print $zone_group['class']; ?> zone">
  <hgroup>
    <h2>Zone <?php print $zone_id;?></h2>
    <h3>Fare <?php print $zone_group['fare']; ?></h3>
    <?php print $zone_group['remove_link']; ?>
  </hgroup>
  <?php foreach ($zone_group['zones'] as $zone) { ?>
    <p><?php print $zone->route_long_name; ?>
       <?php if (sizeof($zone_group['zones']) > 1) { print $zone->remove_link; }?>
    </p>
  <?php }?>
</div>
<?php } ?>

<h2>Create a new zone</h2>