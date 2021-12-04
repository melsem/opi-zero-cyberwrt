module("luci.controller.bootconfig", package.seeall)

function index()

   entry({"admin", "system", "bootconfig"}, template("boot-config/bootconfig"), _("Boot-config"),88).acl_depends={ "unauthenticated" }

end
