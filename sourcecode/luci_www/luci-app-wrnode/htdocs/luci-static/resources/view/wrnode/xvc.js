'use strict';
'require view';
'require form';

// Project code format is tabs, not spaces
return view.extend({
	render: function() {
		let m, s, o;

		/*
		The first argument to form.Map() maps to the configuration file available
		via uci at /etc/config/. In this case, 'example' maps to /etc/config/example.

		If the file is completely empty, the form sections will indicate that the
		section contains no values yet. As such, your package installation (LuCI app
		or software that the app configures) should lay down a basic configuration
		file with all the needed sections.

		The relevant ACL path for reading a configuration with UCI this way is
		read > uci > ["example"]

		The relevant ACL path for writing back the configuration is
		write > uci > ["example"]
		*/
		m = new form.Map('xvc', _('Xilinx Virtual Cable'));

		s = m.section(form.NamedSection, 'config', _('Run options'));
		s.option(form.Value, 'port', _('TCP/IP port'))
		
		o = s.option(form.ListValue, 'verbosity', _('Verbosity level'))
		o.value('info', 'INFO');
		o.value('debug', 'DEBUG');
		o.default = 'info';
		
		return m.render();
	},
});
