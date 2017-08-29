// Use pugInModule from lazyloader.js to add controllers

plugInModule.factory('snmpBackend',function($http) {
})

.controller('snmpCntrlr', ['$scope', '$http', '$timeout', 'snmpBackend',function($scope, $http, $timeout, snmpBackend) {

	// Helper function to easily manage requests and posts to the native interface
	function libcg_page_cb(identifier, json_data, callback_handler_success, callback_handler_error){
		if(identifier != null && identifier != undefined){
			var cb_url = "/api/libcg/" + identifier;
			$.ajax({
				url: cb_url,
				type: "POST",
				data: JSON.stringify(json_data),
				dataType: "json",
				contentType: "application/json; charset=utf-8",
				success: callback_handler_success,
				error: callback_handler_error
			});
		}
	}

	$scope.handle_snmp_settings_success = function(data) {
		var overview = $.parseJSON(JSON.stringify(data));
		$scope.data.isEnabled = overview.isEnabled;
		$scope.data.community = overview.community;
		$scope.data.sysContact = overview.sysContact;
		$scope.data.sysName = overview.sysName;
		$scope.data.sysLocation = overview.sysLocation;
		$scope.data.sysDescr = overview.sysDescr;
		trapServer = overview.trapServer1;
		$scope.data.trapServer1 = trapServer.substring(0,trapServer.indexOf(' '));
		$scope.data.trapCommunity1 = trapServer.substring(trapServer.indexOf(' ')+1);;
		$scope.data.trapEnabled1 = (overview.trapUpDown1 == "yes");
		trapServer = overview.trapServer2;
		$scope.data.trapServer2 = trapServer.substring(0,trapServer.indexOf(' '));
		$scope.data.trapCommunity2 = trapServer.substring(trapServer.indexOf(' ')+1);;
		$scope.data.trapEnabled2 = (overview.trapUpDown2 == "yes");
		trapServer = overview.trapServer3;
		$scope.data.trapServer3 = trapServer.substring(0,trapServer.indexOf(' '));
		$scope.data.trapCommunity3 = trapServer.substring(trapServer.indexOf(' ')+1);;
		$scope.data.trapEnabled3 = (overview.trapUpDown3 == "yes");
//		$('#SnmpStatus').html("Status  : <strong>Settings loaded</strong>");
		$scope.saveddata = angular.copy($scope.data);
		$scope.$apply();
	}

	$scope.handle_json_cb_error = function(data) {
		console.log("Got Error: " + JSON.stringify(data));
	}

	$scope.btnSnmpSave = function() {
		console.log("Saving settings ...");
//		$('#SnmpStatus').html("Status  : <strong>Saving settings ..</strong>");
		var data = angular.copy($scope.data);
		data.trapUpDown1 = (data.trapEnabled1?"yes":"no");
		data.trapUpDown2 = (data.trapEnabled2?"yes":"no");
		data.trapUpDown3 = (data.trapEnabled3?"yes":"no");
		data.trapServer1 = $scope.data.trapServer1 + " " + $scope.data.trapCommunity1;
		data.trapServer2 = $scope.data.trapServer2 + " " + $scope.data.trapCommunity2;
		data.trapServer3 = $scope.data.trapServer3 + " " + $scope.data.trapCommunity3;
		libcg_page_cb("set_snmp_settings_cb", data, $scope.handle_snmp_settings_success, $scope.handle_json_cb_error);
	}

	$scope.btnSnmpCancel = function() {
//		$('#SnmpStatus').html("Status  : <strong>Loading settings ..</strong>");
		$scope.data = angular.copy($scope.saveddata);
	}

	$scope.loadSettings = function() {
//		$('#SnmpStatus').html("Status  : <strong>Loading settings ..</strong>");
		$scope.data={};
		$scope.saveddata = angular.copy($scope.data);
		// load the values for the settings fields
		libcg_page_cb("get_snmp_settings_cb", {}, $scope.handle_snmp_settings_success, $scope.handle_json_cb_error);
	}

	$scope.isClean = function() {
		return angular.equals($scope.data, $scope.saveddata);
	}


	$scope.loadSettings();
}]);
