if tup.getconfig("NO_FASM") ~= "" then return end
tup.rule("QJULIA.ASM", "fasm %f %o " .. tup.getconfig("KPACK_CMD"), "qjulia")
