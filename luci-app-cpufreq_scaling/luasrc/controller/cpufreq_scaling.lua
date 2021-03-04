
module("luci.controller.cpufreq_scaling", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/cpufreq_scaling") then
		return
	end

	entry({"admin", "services", "cpufreq_scaling"}, cbi("cpufreq_scaling"), _("CPU Freq"), 900).dependent=false
end

