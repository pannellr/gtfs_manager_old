(function ($) {
    
Drupal.behaviors.polygonCanvas = {
  attach: function(){
	
    var polygon, map;
    var markers = [];
    var path = new google.maps.MVCArray;

    // Creating a map
    var options = {
      zoom: 7,
      draggableCursor: 'crosshair', 
      draggingCursor: 'crosshair', 
      center: new google.maps.LatLng(45.3824, -63.1986),
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };

$('.form-radio').change(function(){
    $('.polygon-coordinates').hide();
    map = new google.maps.Map(
	document.getElementById('polygon-canvas-container'), options);

      polygon = new google.maps.Polygon({
	  strokeColor: '#0000ff',
	  strokeOpacity: 0.6,
	  strokeWeight: 1,
	  fillColor: '#0000ff',
	  fillOpacity: 0.35
      });
      
      polygon.setMap(map);
      polygon.setPaths(new google.maps.MVCArray([path]));
      
      // Adding a click event to the map object
      google.maps.event.addListener(map, 'click', addPoint);


      
      
	  
      });


      function addPoint(e) {
	  
	  path.insertAt(path.length, e.latLng);
	  
	  var marker = new google.maps.Marker({
	      position: e.latLng,
	      map: map,
	      draggable: true
	  });
	  
	  markers.push(marker);
	  marker.setTitle("#" + path.length);
	  
	  var currentPoints = '';
	  path.forEach(function(point, i) {
	      var separator = (i > 0) ? ',' : '';
	      currentPoints = currentPoints
	          + separator
		  + 'shape_point_sequence:'
		  + i
		  + '|shape_point_lat:'
		  + point.lat()
		  + '|shape_point_long:'
		  + point.lng();
	  });

	  $('.polygon-coordinates').html(currentPoints);

	  google.maps.event.addListener(marker, 'click', function() {
	      marker.setMap(null);
	      for (var i = 0, I = markers.length; i < I && markers[i] != marker; ++i);
	      marker.splice(i, I);
	      path.removeAt(i);
	  });
	  
	  google.maps.event.addListener(marker, 'dragend', function() {
	      for (var i = 0, I = markers.length; i < I && markers[i] != marker; ++i);
	      path.setAt(i, marker.getPosition());
	  });
      }
  }};

})(jQuery);
