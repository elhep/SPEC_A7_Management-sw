'use strict';
'require uci';
'require view';
'require ui';
'require fs';
'require poll';

var isReadonlyView = !L.hasViewPermission() || null;

var pollInterval = 1;

var poll_wrnode_status_timeout = 0
var poll_services_status_timeout = 0

var wrnode_status_json = JSON.parse('{}');
var services_status_json = JSON.parse('{}');

function progressbar(val, max)
	{
		var vn = parseInt(val) || 0;
		var mn = parseInt(max) || 100;
		var pc = Math.floor((100 / mn) * vn);

		return String.format(
			'<div style="width:100%%; max-width:200px; position:relative; border:1px solid #999999">' +
				'<div style="background-color:#CCCCCC; width:%d%%; height:15px">' +
					'<div style="position:absolute; left:0; top:0; text-align:center; width:100%%; color:#000000">' +
						'<small>%d%%</small>' +
					'</div>' +
				'</div>' +
			'</div>', pc, pc
		);
	}

return view.extend({
	handleSaveApply: null,
	handleSave: null,
	handleReset: null,

	load: function () {
		this.pollData();
	},
	
	pollData: function() {
		poll.add(L.bind(function() {
			console.log("poll_wrnode_status_timeout: ", poll_wrnode_status_timeout)
			console.log("poll_services_status_timeout: ", poll_services_status_timeout)
			var json;
			var e;
			
			// ************************************************

			if (poll_wrnode_status_timeout == 0) {
				fs.exec('/root/get_status_wrnode')
				.then(function(ret) {
					wrnode_status_json = JSON.parse(ret['stdout']);
				})
				.then(function(){
					json = wrnode_status_json;
					console.log(wrnode_status_json)
					if (e = document.getElementById('link_present'))
						e.innerHTML = String.format('%s', json['link_present'].toUpperCase());
		
					if (e = document.getElementById('time_sync_status'))
						e.innerHTML = String.format('%s', json['time_sync_status'].toUpperCase());

					if (e = document.getElementById('status_raw'))
						e.innerHTML = String.format('%s', json['status_raw'].toUpperCase());
				});
			}
			else {
				poll_wrnode_status_timeout -= 1
			}
			
			// ************************************************

			if (poll_services_status_timeout == 0) {
				fs.exec('/root/get_status_services')
				.then(function(ret) {
					services_status_json = JSON.parse(ret['stdout']);
					// console.log(services_status_json)
				})
				.then(function() {
					json = services_status_json;
					const services = ['wrnode_xvc', 'wrnode_mqtt_logger'];
					services.forEach(update_state);
	
					function update_state(service) {
						var btn, state;
						if (btn = document.getElementById(service + '_btn')) {
							state = json[service];
							// console.log(service, state)
							if (state.includes('on')) {
								btn.classList.replace('cbi-button-negative', 'cbi-button-positive');
								btn.innerHTML = 'Enabled'
							}
							else {
								btn.classList.replace('cbi-button-positive', 'cbi-button-negative');
								btn.innerHTML = 'Disabled'
							}
						}
					
					}

				});
				
			}
			else {
				poll_services_status_timeout -= 1
			}
			
		}, this), pollInterval);
	},

	service_ctrl_action: function(service, action) {
		return fs.exec('service', ['%s'.format(service), '%s'.format(action)])
		.then(function(ret) {
			console.log(ret);
			if (ret['stderr'])
				throw _('Command failed');
			return true;
		}).catch(function(e) {
			// ui.addNotification(null, E('p', _('Failed to execute "/etc/init.d/%s %s" action: %s').format(name, action, e)));
			throw _('Command failed');
		});
	},

	service_ctrl: function(service, ev) {
		var btn
		if ((btn = document.getElementById(service + '_btn')) == 0) {
			console.log("cannot find element (button)")
			return
		}

		var action = 'start'
		if (btn.classList.contains('cbi-button-positive'))
			var action = 'stop'
		
		poll_services_status_timeout = 3 // delay services status update to prevent button toggling issues

		return this.service_ctrl_action(service, action)
		.then(function() {
			var btn
			if ((btn = document.getElementById(service + '_btn')) == 0) {
				console.log("cannot find element (button)")
				return
			}
			// btn = document.getElementById(service + '_btn')

			if (btn.classList.contains('cbi-button-positive')) { //service is enabled, changing to disabled state
				btn.classList.replace('cbi-button-positive', 'cbi-button-negative');
				btn.innerHTML = 'Disabled'
			}
			else {
				btn.classList.replace('cbi-button-negative', 'cbi-button-positive');
				btn.innerHTML = 'Enabled'
			}
			// btn.disabled = false
		}).catch(function(e) {
			console.log(e)
			// ui.addNotification(null, E('p', _('Unable to save contents: %s').format(e.message)));
		});
	},

	render: function () {		
		var body = E([
			E('h2', _('WR node STATUS and CONTROL'))
		]);

		// ************************************************

		var title = E('h3', _('WR node status'))
		var table = E('table', { 'class': 'table' });
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Link present'), 
			E('td', { 'id': 'link_present', 'class': 'td left' }, '-')
		]));
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Time sync status'), 
			E('td', { 'id': 'time_sync_status', 'class': 'td left' }, '-')
		]));
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Status raw data'), 
			E('td', { 'id': 'status_raw', 'class': 'td left' }, '-')
		]));

		body.appendChild(title);
		body.appendChild(table);

		// ************************************************
		var title = E('h3', _('Services control'))
		var table = E('table', { 'class': 'table' });
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Xilinx Virtual Cable'), 
			E('td', { 'class': 'td left' }, 
				E('button', { 
					'id': 'wrnode_xvc_btn', 
					'class': 'cbi-button cbi-button-negative', 
					'style': 'width: 100px;',
					'click': ui.createHandlerFn(this, 'service_ctrl', 'wrnode_xvc')
				}, 'Disabled')
			)
		]));
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'MQTT logger'), 
			E('td', { 'class': 'td left' }, 
				E('button', { 
					'id': 'wrnode_mqtt_logger_btn', 
					'class': 'cbi-button cbi-button-negative', 
					'style': 'width: 100px;',
					'click': ui.createHandlerFn(this, 'service_ctrl', 'wrnode_mqtt_logger')
				}, 'Disabled')
			)
		]));

		body.appendChild(title);
		body.appendChild(table);

		// ************************************************

		return body;
	}
});