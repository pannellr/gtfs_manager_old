<?php
/**
 * @file template for route stops
 *
 * 
 */
?>
<?php foreach ($variables['stops'] as $stop) { ?>
<h1><?php print $stop->stop_name; ?></h1>

<p><b>Description: </b><?php print $stop->stop_desc; ?></p>
<p><b>Northing UTM: </b><?php print $stop->stop_northing; ?></p>
<p><b>Easting UTM: </b><?php print $stop->stop_easting; ?></p>
<p><b>UTM Zone: </b><?php print $stop->utm_zone; ?></p>
<p><b>Has Wheelchair Access: </b><?php print $stop->wheelchair_boarding; ?></p>
<p><b>Arrival Time: </b><?php print $stop->arrival_time; ?></p>
<p><b>Departure Time: </b><?php print $stop->departure_time; ?></p>
<p><b>Sequence Number: </b><?php print $stop->stop_sequence; ?></p>
<p><b>Calendar: </b><?php print $stop->calendar; ?></p>
<?php print $stop->links; ?> 
<hr />
<?php } ?>
 <?php print $variables['links']; ?>