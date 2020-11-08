
#include <Arduino.h>
#include "DSP_Utils.h"

#define LOG_PRECISION		9
#define LOG_LUT_MASK		((1 << LOG_PRECISION) - 1)
#define LOG_LUT_SIZE		((1 << LOG_PRECISION) + 1)
#define LOG_LUT_FACTOR		(1.0 * (1 << (LOG_PRECISION - 2)))
#define LOG_Q				(23 - LOG_PRECISION)

// These 513 values represent the function (y=2^x) in the interval x -> [-2.0, 2.0]
// The results are in the interval y -> [0.25, 4.0]
// The values are stored in IEEE-754 floating point format; here their bit representation is used.
const FloatBits_t log_512[LOG_LUT_SIZE] =
{
	{ .bits = 0x0}, { .bits = 0x3B387C20}, { .bits = 0x3BB84E23}, { .bits = 0x3C0A1834},
	{ .bits = 0x3C37F286}, { .bits = 0x3C65B61D}, { .bits = 0x3C89B189}, { .bits = 0x3CA07CBC},
	{ .bits = 0x3CB73CB4}, { .bits = 0x3CCDF17C}, { .bits = 0x3CE49B1E}, { .bits = 0x3CFB39A5},
	{ .bits = 0x3D08E68F}, { .bits = 0x3D142AC8}, { .bits = 0x3D1F6985}, { .bits = 0x3D2AA2C9},
	{ .bits = 0x3D35D69C}, { .bits = 0x3D410501}, { .bits = 0x3D4C2DFE}, { .bits = 0x3D575199},
	{ .bits = 0x3D626FD6}, { .bits = 0x3D6D88BB}, { .bits = 0x3D789C4C}, { .bits = 0x3D81D548},
	{ .bits = 0x3D8759C5}, { .bits = 0x3D8CDBA0}, { .bits = 0x3D925ADC}, { .bits = 0x3D97D77B},
	{ .bits = 0x3D9D517F}, { .bits = 0x3DA2C8EB}, { .bits = 0x3DA83DC2}, { .bits = 0x3DADB005},
	{ .bits = 0x3DB31FB8}, { .bits = 0x3DB88CDC}, { .bits = 0x3DBDF775}, { .bits = 0x3DC35F84},
	{ .bits = 0x3DC8C50B}, { .bits = 0x3DCE280E}, { .bits = 0x3DD3888F}, { .bits = 0x3DD8E68F},
	{ .bits = 0x3DDE4212}, { .bits = 0x3DE39B19}, { .bits = 0x3DE8F1A7}, { .bits = 0x3DEE45BE},
	{ .bits = 0x3DF39761}, { .bits = 0x3DF8E691}, { .bits = 0x3DFE3350}, { .bits = 0x3E01BED1},
	{ .bits = 0x3E0462C4}, { .bits = 0x3E070583}, { .bits = 0x3E09A70E}, { .bits = 0x3E0C4766},
	{ .bits = 0x3E0EE68D}, { .bits = 0x3E118483}, { .bits = 0x3E14214A}, { .bits = 0x3E16BCE3},
	{ .bits = 0x3E19574F}, { .bits = 0x3E1BF08F}, { .bits = 0x3E1E88A3}, { .bits = 0x3E211F8E},
	{ .bits = 0x3E23B550}, { .bits = 0x3E2649EA}, { .bits = 0x3E28DD5D}, { .bits = 0x3E2B6FAA},
	{ .bits = 0x3E2E00D2}, { .bits = 0x3E3090D6}, { .bits = 0x3E331FB8}, { .bits = 0x3E35AD78},
	{ .bits = 0x3E383A17}, { .bits = 0x3E3AC596}, { .bits = 0x3E3D4FF6}, { .bits = 0x3E3FD939},
	{ .bits = 0x3E42615F}, { .bits = 0x3E44E868}, { .bits = 0x3E476E57}, { .bits = 0x3E49F32C},
	{ .bits = 0x3E4C76E8}, { .bits = 0x3E4EF98C}, { .bits = 0x3E517B19}, { .bits = 0x3E53FB90},
	{ .bits = 0x3E567AF2}, { .bits = 0x3E58F93F}, { .bits = 0x3E5B7679}, { .bits = 0x3E5DF2A0},
	{ .bits = 0x3E606DB6}, { .bits = 0x3E62E7BC}, { .bits = 0x3E6560B2}, { .bits = 0x3E67D899},
	{ .bits = 0x3E6A4F72}, { .bits = 0x3E6CC53F}, { .bits = 0x3E6F39FF}, { .bits = 0x3E71ADB4},
	{ .bits = 0x3E74205F}, { .bits = 0x3E769201}, { .bits = 0x3E79029A}, { .bits = 0x3E7B722B},
	{ .bits = 0x3E7DE0B6}, { .bits = 0x3E80271D}, { .bits = 0x3E815D5D}, { .bits = 0x3E82931B},
	{ .bits = 0x3E83C857}, { .bits = 0x3E84FD12}, { .bits = 0x3E86314C}, { .bits = 0x3E876505},
	{ .bits = 0x3E88983F}, { .bits = 0x3E89CAF9}, { .bits = 0x3E8AFD34}, { .bits = 0x3E8C2EF0},
	{ .bits = 0x3E8D602E}, { .bits = 0x3E8E90ED}, { .bits = 0x3E8FC130}, { .bits = 0x3E90F0F5},
	{ .bits = 0x3E92203D}, { .bits = 0x3E934F0A}, { .bits = 0x3E947D5A}, { .bits = 0x3E95AB2E},
	{ .bits = 0x3E96D888}, { .bits = 0x3E980567}, { .bits = 0x3E9931CB}, { .bits = 0x3E9A5DB6},
	{ .bits = 0x3E9B8926}, { .bits = 0x3E9CB41E}, { .bits = 0x3E9DDE9D}, { .bits = 0x3E9F08A3},
	{ .bits = 0x3EA03232}, { .bits = 0x3EA15B49}, { .bits = 0x3EA283E8}, { .bits = 0x3EA3AC10},
	{ .bits = 0x3EA4D3C2}, { .bits = 0x3EA5FAFE}, { .bits = 0x3EA721C4}, { .bits = 0x3EA84814},
	{ .bits = 0x3EA96DF0}, { .bits = 0x3EAA9357}, { .bits = 0x3EABB849}, { .bits = 0x3EACDCC7},
	{ .bits = 0x3EAE00D2}, { .bits = 0x3EAF2469}, { .bits = 0x3EB0478E}, { .bits = 0x3EB16A3F},
	{ .bits = 0x3EB28C7F}, { .bits = 0x3EB3AE4D}, { .bits = 0x3EB4CFA9}, { .bits = 0x3EB5F094},
	{ .bits = 0x3EB7110F}, { .bits = 0x3EB83118}, { .bits = 0x3EB950B2}, { .bits = 0x3EBA6FDB},
	{ .bits = 0x3EBB8E96}, { .bits = 0x3EBCACE1}, { .bits = 0x3EBDCABD}, { .bits = 0x3EBEE82B},
	{ .bits = 0x3EC0052B}, { .bits = 0x3EC121BD}, { .bits = 0x3EC23DE2}, { .bits = 0x3EC35999},
	{ .bits = 0x3EC474E4}, { .bits = 0x3EC58FC2}, { .bits = 0x3EC6AA34}, { .bits = 0x3EC7C43A},
	{ .bits = 0x3EC8DDD4}, { .bits = 0x3EC9F704}, { .bits = 0x3ECB0FC8}, { .bits = 0x3ECC2822},
	{ .bits = 0x3ECD4012}, { .bits = 0x3ECE5798}, { .bits = 0x3ECF6EB4}, { .bits = 0x3ED08567},
	{ .bits = 0x3ED19BB0}, { .bits = 0x3ED2B191}, { .bits = 0x3ED3C70A}, { .bits = 0x3ED4DC1B},
	{ .bits = 0x3ED5F0C4}, { .bits = 0x3ED70505}, { .bits = 0x3ED818E0}, { .bits = 0x3ED92C53},
	{ .bits = 0x3EDA3F60}, { .bits = 0x3EDB5206}, { .bits = 0x3EDC6447}, { .bits = 0x3EDD7622},
	{ .bits = 0x3EDE8797}, { .bits = 0x3EDF98A8}, { .bits = 0x3EE0A953}, { .bits = 0x3EE1B99B},
	{ .bits = 0x3EE2C97D}, { .bits = 0x3EE3D8FC}, { .bits = 0x3EE4E818}, { .bits = 0x3EE5F6D0},
	{ .bits = 0x3EE70524}, { .bits = 0x3EE81317}, { .bits = 0x3EE920A6}, { .bits = 0x3EEA2DD4},
	{ .bits = 0x3EEB3A9F}, { .bits = 0x3EEC4709}, { .bits = 0x3EED5311}, { .bits = 0x3EEE5EB8},
	{ .bits = 0x3EEF69FF}, { .bits = 0x3EF074E4}, { .bits = 0x3EF17F6A}, { .bits = 0x3EF2898F},
	{ .bits = 0x3EF39355}, { .bits = 0x3EF49CBB}, { .bits = 0x3EF5A5C2}, { .bits = 0x3EF6AE6A},
	{ .bits = 0x3EF7B6B4}, { .bits = 0x3EF8BE9F}, { .bits = 0x3EF9C62B}, { .bits = 0x3EFACD5A},
	{ .bits = 0x3EFBD42B}, { .bits = 0x3EFCDA9F}, { .bits = 0x3EFDE0B6}, { .bits = 0x3EFEE670},
	{ .bits = 0x3EFFEBCD}, { .bits = 0x3F007867}, { .bits = 0x3F00FAB9}, { .bits = 0x3F017CDE},
	{ .bits = 0x3F01FED4}, { .bits = 0x3F02809D}, { .bits = 0x3F030239}, { .bits = 0x3F0383A7},
	{ .bits = 0x3F0404E8}, { .bits = 0x3F0485FB}, { .bits = 0x3F0506E2}, { .bits = 0x3F05879B},
	{ .bits = 0x3F060828}, { .bits = 0x3F068888}, { .bits = 0x3F0708BC}, { .bits = 0x3F0788C3},
	{ .bits = 0x3F08089E}, { .bits = 0x3F08884C}, { .bits = 0x3F0907CF}, { .bits = 0x3F098725},
	{ .bits = 0x3F0A0650}, { .bits = 0x3F0A854F}, { .bits = 0x3F0B0422}, { .bits = 0x3F0B82CA},
	{ .bits = 0x3F0C0146}, { .bits = 0x3F0C7F98}, { .bits = 0x3F0CFDBE}, { .bits = 0x3F0D7BB9},
	{ .bits = 0x3F0DF989}, { .bits = 0x3F0E772E}, { .bits = 0x3F0EF4A9}, { .bits = 0x3F0F71F9},
	{ .bits = 0x3F0FEF1F}, { .bits = 0x3F106C1A}, { .bits = 0x3F10E8EB}, { .bits = 0x3F116592},
	{ .bits = 0x3F11E20F}, { .bits = 0x3F125E62}, { .bits = 0x3F12DA8B}, { .bits = 0x3F13568A},
	{ .bits = 0x3F13D260}, { .bits = 0x3F144E0D}, { .bits = 0x3F14C990}, { .bits = 0x3F1544EA},
	{ .bits = 0x3F15C01A}, { .bits = 0x3F163B22}, { .bits = 0x3F16B601}, { .bits = 0x3F1730B7},
	{ .bits = 0x3F17AB44}, { .bits = 0x3F1825A8}, { .bits = 0x3F189FE4}, { .bits = 0x3F1919F8},
	{ .bits = 0x3F1993E3}, { .bits = 0x3F1A0DA7}, { .bits = 0x3F1A8742}, { .bits = 0x3F1B00B5},
	{ .bits = 0x3F1B7A00}, { .bits = 0x3F1BF323}, { .bits = 0x3F1C6C1F}, { .bits = 0x3F1CE4F3},
	{ .bits = 0x3F1D5DA0}, { .bits = 0x3F1DD625}, { .bits = 0x3F1E4E83}, { .bits = 0x3F1EC6BA},
	{ .bits = 0x3F1F3ECA}, { .bits = 0x3F1FB6B2}, { .bits = 0x3F202E74}, { .bits = 0x3F20A610},
	{ .bits = 0x3F211D84}, { .bits = 0x3F2194D2}, { .bits = 0x3F220BF9}, { .bits = 0x3F2282FA},
	{ .bits = 0x3F22F9D5}, { .bits = 0x3F237089}, { .bits = 0x3F23E718}, { .bits = 0x3F245D80},
	{ .bits = 0x3F24D3C2}, { .bits = 0x3F2549DF}, { .bits = 0x3F25BFD6}, { .bits = 0x3F2635A7},
	{ .bits = 0x3F26AB53}, { .bits = 0x3F2720D9}, { .bits = 0x3F27963A}, { .bits = 0x3F280B76},
	{ .bits = 0x3F28808C}, { .bits = 0x3F28F57E}, { .bits = 0x3F296A4A}, { .bits = 0x3F29DEF2},
	{ .bits = 0x3F2A5374}, { .bits = 0x3F2AC7D2}, { .bits = 0x3F2B3C0C}, { .bits = 0x3F2BB021},
	{ .bits = 0x3F2C2411}, { .bits = 0x3F2C97DD}, { .bits = 0x3F2D0B85}, { .bits = 0x3F2D7F09},
	{ .bits = 0x3F2DF268}, { .bits = 0x3F2E65A4}, { .bits = 0x3F2ED8BC}, { .bits = 0x3F2F4BB0},
	{ .bits = 0x3F2FBE80}, { .bits = 0x3F30312C}, { .bits = 0x3F30A3B5}, { .bits = 0x3F31161A},
	{ .bits = 0x3F31885C}, { .bits = 0x3F31FA7B}, { .bits = 0x3F326C77}, { .bits = 0x3F32DE4F},
	{ .bits = 0x3F335004}, { .bits = 0x3F33C197}, { .bits = 0x3F343306}, { .bits = 0x3F34A453},
	{ .bits = 0x3F35157D}, { .bits = 0x3F358684}, { .bits = 0x3F35F769}, { .bits = 0x3F36682B},
	{ .bits = 0x3F36D8CB}, { .bits = 0x3F374949}, { .bits = 0x3F37B9A4}, { .bits = 0x3F3829DE},
	{ .bits = 0x3F3899F5}, { .bits = 0x3F3909EA}, { .bits = 0x3F3979BD}, { .bits = 0x3F39E96F},
	{ .bits = 0x3F3A58FF}, { .bits = 0x3F3AC86D}, { .bits = 0x3F3B37B9}, { .bits = 0x3F3BA6E4},
	{ .bits = 0x3F3C15EE}, { .bits = 0x3F3C84D6}, { .bits = 0x3F3CF39D}, { .bits = 0x3F3D6243},
	{ .bits = 0x3F3DD0C8}, { .bits = 0x3F3E3F2B}, { .bits = 0x3F3EAD6E}, { .bits = 0x3F3F1B90},
	{ .bits = 0x3F3F8991}, { .bits = 0x3F3FF771}, { .bits = 0x3F406531}, { .bits = 0x3F40D2D0},
	{ .bits = 0x3F41404F}, { .bits = 0x3F41ADAD}, { .bits = 0x3F421AEB}, { .bits = 0x3F428808},
	{ .bits = 0x3F42F506}, { .bits = 0x3F4361E3}, { .bits = 0x3F43CEA0}, { .bits = 0x3F443B3D},
	{ .bits = 0x3F44A7BA}, { .bits = 0x3F451418}, { .bits = 0x3F458055}, { .bits = 0x3F45EC73},
	{ .bits = 0x3F465872}, { .bits = 0x3F46C451}, { .bits = 0x3F473010}, { .bits = 0x3F479BB0},
	{ .bits = 0x3F480731}, { .bits = 0x3F487292}, { .bits = 0x3F48DDD4}, { .bits = 0x3F4948F7},
	{ .bits = 0x3F49B3FB}, { .bits = 0x3F4A1EE0}, { .bits = 0x3F4A89A7}, { .bits = 0x3F4AF44E},
	{ .bits = 0x3F4B5ED7}, { .bits = 0x3F4BC940}, { .bits = 0x3F4C338C}, { .bits = 0x3F4C9DB8},
	{ .bits = 0x3F4D07C7}, { .bits = 0x3F4D71B6}, { .bits = 0x3F4DDB88}, { .bits = 0x3F4E453B},
	{ .bits = 0x3F4EAED0}, { .bits = 0x3F4F1847}, { .bits = 0x3F4F819F}, { .bits = 0x3F4FEADA},
	{ .bits = 0x3F5053F7}, { .bits = 0x3F50BCF6}, { .bits = 0x3F5125D7}, { .bits = 0x3F518E9A},
	{ .bits = 0x3F51F740}, { .bits = 0x3F525FC8}, { .bits = 0x3F52C832}, { .bits = 0x3F53307F},
	{ .bits = 0x3F5398AF}, { .bits = 0x3F5400C1}, { .bits = 0x3F5468B6}, { .bits = 0x3F54D08D},
	{ .bits = 0x3F553848}, { .bits = 0x3F559FE5}, { .bits = 0x3F560765}, { .bits = 0x3F566EC9},
	{ .bits = 0x3F56D60F}, { .bits = 0x3F573D39}, { .bits = 0x3F57A446}, { .bits = 0x3F580B36},
	{ .bits = 0x3F587209}, { .bits = 0x3F58D8C0}, { .bits = 0x3F593F5A}, { .bits = 0x3F59A5D8},
	{ .bits = 0x3F5A0C3A}, { .bits = 0x3F5A727F}, { .bits = 0x3F5AD8A8}, { .bits = 0x3F5B3EB4},
	{ .bits = 0x3F5BA4A4}, { .bits = 0x3F5C0A79}, { .bits = 0x3F5C7031}, { .bits = 0x3F5CD5CD},
	{ .bits = 0x3F5D3B4E}, { .bits = 0x3F5DA0B2}, { .bits = 0x3F5E05FB}, { .bits = 0x3F5E6B28},
	{ .bits = 0x3F5ED039}, { .bits = 0x3F5F352F}, { .bits = 0x3F5F9A09}, { .bits = 0x3F5FFEC7},
	{ .bits = 0x3F60636A}, { .bits = 0x3F60C7F2}, { .bits = 0x3F612C5E}, { .bits = 0x3F6190AF},
	{ .bits = 0x3F61F4E5}, { .bits = 0x3F625900}, { .bits = 0x3F62BCFF}, { .bits = 0x3F6320E4},
	{ .bits = 0x3F6384AD}, { .bits = 0x3F63E85C}, { .bits = 0x3F644BF0}, { .bits = 0x3F64AF69},
	{ .bits = 0x3F6512C7}, { .bits = 0x3F65760A}, { .bits = 0x3F65D933}, { .bits = 0x3F663C41},
	{ .bits = 0x3F669F35}, { .bits = 0x3F67020E}, { .bits = 0x3F6764CD}, { .bits = 0x3F67C771},
	{ .bits = 0x3F6829FB}, { .bits = 0x3F688C6B}, { .bits = 0x3F68EEC1}, { .bits = 0x3F6950FC},
	{ .bits = 0x3F69B31E}, { .bits = 0x3F6A1525}, { .bits = 0x3F6A7712}, { .bits = 0x3F6AD8E6},
	{ .bits = 0x3F6B3A9F}, { .bits = 0x3F6B9C3F}, { .bits = 0x3F6BFDC5}, { .bits = 0x3F6C5F31},
	{ .bits = 0x3F6CC083}, { .bits = 0x3F6D21BC}, { .bits = 0x3F6D82DB}, { .bits = 0x3F6DE3E1},
	{ .bits = 0x3F6E44CD}, { .bits = 0x3F6EA5A0}, { .bits = 0x3F6F065A}, { .bits = 0x3F6F66FA},
	{ .bits = 0x3F6FC781}, { .bits = 0x3F7027EF}, { .bits = 0x3F708843}, { .bits = 0x3F70E87F},
	{ .bits = 0x3F7148A1}, { .bits = 0x3F71A8AB}, { .bits = 0x3F72089B}, { .bits = 0x3F726873},
	{ .bits = 0x3F72C832}, { .bits = 0x3F7327D8}, { .bits = 0x3F738765}, { .bits = 0x3F73E6DA},
	{ .bits = 0x3F744636}, { .bits = 0x3F74A579}, { .bits = 0x3F7504A4}, { .bits = 0x3F7563B6},
	{ .bits = 0x3F75C2B0}, { .bits = 0x3F762191}, { .bits = 0x3F76805A}, { .bits = 0x3F76DF0B},
	{ .bits = 0x3F773DA4}, { .bits = 0x3F779C24}, { .bits = 0x3F77FA8C}, { .bits = 0x3F7858DC},
	{ .bits = 0x3F78B714}, { .bits = 0x3F791534}, { .bits = 0x3F79733C}, { .bits = 0x3F79D12C},
	{ .bits = 0x3F7A2F04}, { .bits = 0x3F7A8CC5}, { .bits = 0x3F7AEA6D}, { .bits = 0x3F7B47FE},
	{ .bits = 0x3F7BA578}, { .bits = 0x3F7C02D9}, { .bits = 0x3F7C6023}, { .bits = 0x3F7CBD56},
	{ .bits = 0x3F7D1A71}, { .bits = 0x3F7D7774}, { .bits = 0x3F7DD460}, { .bits = 0x3F7E3135},
	{ .bits = 0x3F7E8DF2}, { .bits = 0x3F7EEA99}, { .bits = 0x3F7F4728}, { .bits = 0x3F7FA39F},
	{ .bits = 0x3F800000},
};
