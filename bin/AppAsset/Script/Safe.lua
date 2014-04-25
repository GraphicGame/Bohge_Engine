-- ��֧��Lua 5.2��
assert(_VERSION == "Lua 5.2")

-- ȫ�ֻ�����ע����е�����ֵ����lua.h��
local LUA_RIDX_GLOBALS = 2

-- ��ȫtable��metatable��־
local SAFE_TABLE_FLAG = ".SAFETABLE"

-- ����ȫ�ְ�ȫ��������
local function SetupGlobal()

	-- ��ȡע���
	local reg = debug.getregistry()

	local env = {}			-- �»���table
	local proxy = {}		-- ����table
	local mt = {}			-- metatable

	-- ��������
	mt.__index = proxy
	mt.__newindex = function() print("cannot modify global enviroment!") end
	mt.__len = function() return #proxy end
	mt.__pairs = function() return pairs(proxy) end
	mt.__ipairs = function() return ipairs(proxy) end

	-- ����metatable
	mt.__metatable = 0

	-- ���Ϊ��ȫtable
	mt[SAFE_TABLE_FLAG] = true

	-- ��ȡ�ɻ���
	local old_env = reg[LUA_RIDX_GLOBALS]

	-- �����»�����metatable
	setmetatable(env, mt)

	-- �����»���
	_ENV = env

	-- ��ȫ��Ĭ�ϻ���Ҳ��Ϊ�»���
	reg[LUA_RIDX_GLOBALS] = env

	-- ���ش���table�;ɻ���
	return proxy, old_env

end

-- �½�һ���а�ȫ������table
local function CreateSafeTable(base)

	local new = {}			-- ��table
	local mt = {}			-- metatable

	-- ���û��ָ��base���½�һ����table
	local proxy = (type(base) == "table") and base or {}

	-- ��������
	mt.__index = proxy
	mt.__newindex = function() print("cannot modify safe table!") end
	mt.__len = function() return #proxy end
	mt.__pairs = function() return pairs(proxy) end
	mt.__ipairs = function() return ipairs(proxy) end

	-- ����metatable
	mt.__metatable = 0

	-- ���Ϊ��ȫtable
	mt[SAFE_TABLE_FLAG] = true

	-- ������table��metatable
	setmetatable(new, mt)

	-- ������table�Ͷ�Ӧ�Ĵ���table
	return new, proxy

end

-- ����ȫ�ֱ���
local proxy, old_env = SetupGlobal()

-- �����︴����Ҫ�������»���ʹ�õ�Luaԭ��ȫ�ֱ����ͺ���
-- �����ε�ԭ��ȫ�ֱ����ͺ����У�
--	_G			Lua 5.2�Ƽ�ʹ��_ENV������Ը�����Ҫ��������Ϊ_ENV��
--	dofile		�ҵĹ�����Ҫ�����ļ�ϵͳ����û���������������԰�ȫ����û��Ӱ��
--	loadfile	�ҵĹ�����Ҫ�����ļ�ϵͳ����û���������������԰�ȫ����û��Ӱ��
--	rawequal	��Ҫ���ǣ���Ӧ��ֱ�Ӳ�����ȫtable
--	rawget		��Ҫ���ǣ���Ӧ��ֱ�Ӳ�����ȫtable
--	rawlen		��Ҫ���ǣ���Ӧ��ֱ�Ӳ�����ȫtable
--	rawset		��Ҫ���ǣ���Ӧ��ֱ�Ӳ�����ȫtable
--	require		�ҵĹ�����Ҫ�����ļ�ϵͳ����û���������������԰�ȫ����û��Ӱ��
proxy._VERSION = old_env._VERSION
proxy.assert = old_env.assert
proxy.collectgarbage = old_env.collectgarbage
proxy.error = old_env.error
proxy.getmetatable = old_env.getmetatable
proxy.ipairs = old_env.ipairs
proxy.load = old_env.load
proxy.next = old_env.next
proxy.pairs = old_env.pairs
proxy.pcall = old_env.pcall
proxy.print = old_env.print
proxy.select = old_env.select
proxy.setmetatable = old_env.setmetatable
proxy.tostring = old_env.tostring
proxy.tonumber = old_env.tonumber
proxy.type = old_env.type
proxy.xpcall = old_env.xpcall

-- �����ﵼ�����»���ʹ�õ�Luaԭ��ȫ��table��������Ϊֻ��table��
-- �����ε�ԭ��ȫ��table�У�
--	coroutine	�ҵĹ����ﲻ��Ҫcoroutine����û���������������԰�ȫ����û��Ӱ��
--	debug		������Ӱ�찲ȫ�ԣ���������
--	io			�ҵĹ�����Ҫ�����ļ�ϵͳ����û���������������԰�ȫ����û��Ӱ��
--	os			�ҵĹ����ﲻ��Ҫos����û���������������԰�ȫ����û��Ӱ��
--	package		�ҵĹ�����Ҫ�����ļ�ϵͳ����û���������������԰�ȫ����û��Ӱ��
proxy.bit32 = CreateSafeTable(old_env.bit32)
proxy.math = CreateSafeTable(old_env.math)
proxy.string = CreateSafeTable(old_env.string)
proxy.table = CreateSafeTable(old_env.table)

-- ʵ�ְ�ȫ���rawequal
proxy.rawequal = function(v1, v2)

	-- �����ʵ��metatable
	local mt1 = old_env.debug.getmetatable(v1)
	local mt2 = old_env.debug.getmetatable(v2)

	-- ����ǰ�ȫtable��ʹ�ô���table
	if mt1 and mt1[SAFE_TABLE_FLAG] then
		v1 = mt1.__index
	end
	if mt2 and mt2[SAFE_TABLE_FLAG] then
		v2 = mt2.__index
	end

	-- ����ԭʼrawequal
	return old_env.rawequal(v1, v2)

end

-- ʵ�ְ�ȫ���rawget
proxy.rawget = function(t, k)

	-- �����ʵ��metatable
	local mt = old_env.debug.getmetatable(t)

	-- ����ǰ�ȫtable��ʹ�ô���table
	if mt and mt[SAFE_TABLE_FLAG] then
		t = mt.__index
	end

	-- ����ԭʼrawget
	return old_env.rawget(t, k)

end

-- ʵ�ְ�ȫ���rawlen
proxy.rawlen = function(v)

	-- �����ʵ��metatable
	local mt = old_env.debug.getmetatable(v)

	-- ����ǰ�ȫtable��ʹ�ô���table
	if mt and mt[SAFE_TABLE_FLAG] then
		v = mt.__index
	end

	-- ����ԭʼrawlen
	return old_env.rawlen(v)

end

-- ʵ�ְ�ȫ���rawset
proxy.rawset = function(t, k, v)

	-- �����ʵ��metatable
	local mt = old_env.debug.getmetatable(t)

	-- ����ǰ�ȫtable��ʹ�ô���table
	if mt and mt[SAFE_TABLE_FLAG] then
		t = mt.__index
	end

	-- ����ԭʼrawset
	return old_env.rawset(t, k, v)

end

-- ��������Զ���һЩ�Լ�������

-- �ű��ļ�װ���б�
local loaded_proxy
proxy.LOADED, loaded_proxy = CreateSafeTable()

-- ����ű��ļ�
proxy.import = function(s)

	-- ����Ѿ��������򷵻�true
	if LOADED[s] ~= nil then
		return true
	end

	-- װ���ļ�
	local f, msg = old_env.loadfile(s)

	-- ���װ��ʧ�ܣ��������
	if not f then
		old_env.io.stderr:write(msg)
		return false
	end

	-- ����ִ�иýű�
	local r, msg = pcall(f)

	-- ���ִ�й����г����������
	if not r then
		old_env.io.stderr:write(msg)
		return false
	end

	-- ��¼�ļ�����װ���б�
	loaded_proxy[s] = f

	-- �ɹ�
	return true

end

-- ������磨����ָ����main.lua�������Ѿ���ʼ���������ˣ�û�취��safe.lua��ֱ�Ӹ��ģ���û�ҵ��취��
-- ������ﷵ���»�����main.lua��main.lua��Ҫ��װ������ļ�����Լ��Ļ�����Ϊ���»���
-- ����C��һ���ǲ���Ҫ�ģ�����main.lua������һ�п��Զ���C�����
do return _ENV end