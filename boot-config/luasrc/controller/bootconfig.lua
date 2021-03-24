module("luci.controller.bootconfig", package.seeall)

function index()
	local page
	page = entry({"admin","system","bootconfig"},call("action_tryme"),_("Boot-config"),88)
	page.dependent = true

end

function action_tryme() luci.sys.call("cgi-bin/modules/boot-config/index.cgi 55") end
