function Test( bResult, szMessage )
	print( szMessage.." ........ "..( bResult and "OK" or "Failed" ) );
end

CApplicationHandler = class(IApplicationHandler);
function CApplicationHandler:OnTestPureVirtual(
	e, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
    
	gdb()
	Test( e == 1234, "Test enum value to Script" );

	Test( v0 == -123, "Test int8 value to Script" );
	Test( v1 == -12345, "Test int16 value to Script" );
	Test( v2 == -12345678, "Test int32 value to Script" );
	Test( v3 == -1234567891011, "Test int64 value to Script" );
	Test( v4 == -123456789, "Test long value to Script" );

	Test( v5 == 123, "Test uint8 value to Script" );
	Test( v6 == 12345, "Test uint16 value to Script" );
	Test( v7 == 12345678, "Test uint32 value to Script" );
	Test( v8 == 1234567891011, "Test uint64 value to Script" );
	Test( v9 == 123456789, "Test unsigned long value to Script" );

	Test( v10 == 1234567.0, "Test float value to Script" );
	Test( v11 == 123456789101112.0, "Test double value to Script" );

	Test( v12 == "abcdefg", "Test const char* value to Script" );
	Test( v13 == "abcdefg", "Test const wchar_t* value to Script" );
	return "OK"
end

function CApplicationHandler:OnTestNoParamPureVirtual()
	return "OK"
end

g_handler = CApplicationHandler:new();
g_App = CApplication.GetInst();

function g_App:TestVirtual( v0, v1, v2 )
	Test( v0:szName() == "sampler" and v0:nID() == 12345, "Test object value to Script" );
	Test( v1:szName() == "sampler" and v1:nID() == 12345, "Test object reference to Script" );
	Test( v2:szName() == "sampler" and v2:nID() == 12345, "Test object pointer to Script" );
	Test( v2 == v1 and v0 ~= v2, "Test object value copy to Script" );
	return v1
end

function StartApplication( name, id )
	local config = SApplicationConfig:new();
	config:szName( name );
	config:nID( id );

	Test( g_App:TestCallObject(g_handler, config, config, config) == config, "Test return obj" );
	Test( g_App:TestCallPOD(1234, -123, -12345, -12345678, -1234567891011, -123456789, 123, 12345, 
		12345678, 1234567891011, 123456789, 1234567, 123456789101112, "abcdefg", "abcdefg") == "OK",
		"Test return string" );
	Test( g_App:TestNoParamFunction() == "OK", "Test function without parameter" );
end

print( "Test lua loaded" );
