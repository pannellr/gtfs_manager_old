<?php
/*
 * @file template for showing all agencies
 */
?>

<div class="all-agencies-view">
  <?php print implode('<br />', $links); ?>
  <table>
    <thead>
      <tr>
      	<th>Name</th>
      	<th>URL</th>
      	<th>Phone</th>
      	<th>email</th>
      	<th>Fare URL</th>
      	<th>Features</th>
      	<th>Options</th>
     </tr>
   </thead>
   </tbody>

   <?php foreach ($agencies as $agency) { ?>
     <tr>
     	<td><?php print stripslashes($agency->agency_name); ?></td>
     	<td><?php print $agency->agency_url; ?></td>
     	<td><?php print $agency->agency_phone; ?></td>
     	<td><?php print $agency->agency_email; ?></td>
     	<td><?php print $agency->agency_fare_url ?></td>
   		<td><?php print $agency->features; ?></td>
   		<td><?php print implode('<br />', $agency->options); ?></td>
   	</tr>
   <?php } ?>
    </tbody>
  </table>
</div>