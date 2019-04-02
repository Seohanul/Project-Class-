#pragma once

constexpr int MAX_USER		= 10;
constexpr int WORLD_WIDTH	= 8;
constexpr int WORLD_HEIGHT	= 8;

constexpr int SERVER_PORT = 3500;


constexpr int CS_UP			= 1;
constexpr int CS_DOWN		= 2;
constexpr int CS_LEFT		= 3;
constexpr int CS_RIGHT		= 4;


constexpr int SC_LOGIN_OK			= 1;
constexpr int SC_PUT_PLAYER			= 2;
constexpr int SC_REMOVE_PLAYER		= 3;
constexpr int SC_POS				= 4;

#pragma pack(push, 1)
struct sc_packet_pos
{
	char size;
	char type;
	char id;
	char x, y;
};

struct sc_packet_remove_player
{
	char size;
	char type;
	char id;
};


struct sc_packet_login_ok
{
	char size;
	char type;
	char id;
};


struct sc_packet_put_player
{
	char size;
	char type;
	char id;
	char x, y;
};

struct cs_packet_up
{
	char	size;
	char	type;
};

struct cs_packet_down
{
	char	size;
	char	type;
};

struct cs_packet_left
{
	char	size;
	char	type;
};

struct cs_packet_right
{
	char	size;
	char	type;
};


#pragma pack(pop, 1)