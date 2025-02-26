'use strict';
'require uci';
'require view';
'require ui';
'require fs';
'require form';

return view.extend({
	handleSaveApply: null,
	handleSave: null,
	handleReset: null,



	uploadFpgaGateware: function(ev) {
		return ui.uploadFile('/tmp/fpga_gw.bin', ev.target.firstChild)
	},

	programFpga: function(ev) {
		console.log('programFpga')
		return fs.exec('/root/openocd_fpga/program_smt2nc.sh', [ '/tmp/fpga_gw.bin' ])
		// return fs.exec_direct('/root/openocd_fpga/program_smt2nc.sh', [ '/tmp/fpga_gw.bin' ])
		.then(function(res) { 
			console.log(res);
			ui.addNotification(null, E('p', _('%s').format(res['stderr'])));
			// return reply; 
		});
	},

	render: function () {

		var body = E([
			E('h2', _('SPI FLASH operations'))
		]);

		var title = E('h3', _('Programming'))
		var table = E('table', { 'class': 'table' });
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Upload Binary file to WR_NODE'), 
			E('td', { 'class': 'td left' }, 
				E('button', { 
					'id': 'upload_binary_btn', 
					'class': 'cbi-button cbi-button-action', 
					'style': 'width: 100px;',
					'click':  ui.createHandlerFn(this, 'uploadFpgaGateware')
				}, 'UPLOAD')
			)
		]));
		table.appendChild(E('tr', { 'class': 'tr' }, [
			E('td', { 'class': 'td left', 'width': '33%' }, 'Program SPI FLASH'), 
			E('td', { 'class': 'td left' }, 
				E('button', { 
					'id': 'run_action_btn', 
					'class': 'cbi-button cbi-button-save', 
					'style': 'width: 100px;',
					'click':  ui.createHandlerFn(this, 'programFpga')
				}, 'RUN')
			)
		]));

		body.appendChild(title);
		body.appendChild(table);

		return body;
	}
});