<?php
/*
 * @file template for showing all routes for given agency
 */
?>


<div class="agency-routes-view">
  <div class="agency-routes-view-links"><?php print implode(' | ', $variables['links']); ?></div>
  <table>
    <thead>
      <tr>
      	<th>Name</th>
      	<th>Short Name</th>
      	<th>Desc</th>
      	<th>Type</th>
      	<th>Vehicle type</th>
      	<th>Features</th>
      	<th>Option</th>
     </tr>
   </thead>
   <tbody>

	 <?php foreach ($variables['routes'] as $route) {?>
	 <tr>
	   <td><?php print $route->route_long_name; ?></td>
	   <td><?php print $route->route_short_name; ?></td>
	   <td><?php print $route->route_desc; ?></td>
	   <td><?php print $route->route_service_type; ?></td>
	   <td><?php print $route->transportation_type_id; ?></td>
	   <td><?php print $route->features; ?></td>
	   <td><?php print implode('<br />', $route->links); ?></td>
     </tr>
     <?php } ?>
   </tbody>
 </table>
</div>