//
// Created by Jon on 7/4/2021.
//

#include <spdlog/spdlog.h>
#include "cpuimpl.h"
#include "cpu.h"
#include "mm.h"
#include "ops.h"

using namespace GBCPP;

CpuImpl::CpuImpl(MemNS::MemMgr& mem_mgr) :	reg(), state(), a(*reg->a), b(*reg->b), c(*reg->c), d(*reg->d), e(*reg->e), h(*reg->h), l(*reg->l), f(*reg->f),
											af(*reg->af), bc(*reg->bc), de(*reg->de), hl(*reg->hl), sp(*reg->sp), pc(*reg->pc) {
	CpuImpl::m = mem_mgr;
}

void CpuImpl::lookup_and_execute()
{
	state->lastop = m[pc];
	switch (state->lastop)
	{
		case 0x00:break;
		case 0x01:ld16(bc, get16(pc+1));
			break;
		case 0x02:ld_i(bc);
			break;
		case 0x03:inc16(bc);
			break;
		case 0x04:inc(b);
			break;
		case 0x05:dec(b);
			break;
		case 0x06:ld(b, get8(pc+1));
			break;
		case 0x07:rlca();
			break;
		case 0x08:m[get16(pc+1)] = (sp & 0xFF); m[get16(pc+2)] = (sp & 0xFF00) >> 8;
			break;
		case 0x09:add16(bc);
			break;
		case 0x0A:ld(a, m[bc]);
			break;
		case 0x0B:dec16(bc);
			break;
		case 0x0C:inc(c);
			break;
		case 0x0D:dec(c);
			break;
		case 0x0E:ld(c, get8(pc+1));
			break;
		case 0x0F:rrca();
			break;
		case 0x10:state->stopped = true;
			break;
		case 0x11:ld16(de, get16(pc+1));
			break;
		case 0x12:ld_i(de);
			break;
		case 0x13:inc16(de);
			break;
		case 0x14:inc(d);
			break;
		case 0x15:dec(d);
			break;
		case 0x16:ld(d, get8(pc+1));
			break;
		case 0x17:rla();
			break;
		case 0x18:jr(true);
			break;
		case 0x19:add16(de);
			break;
		case 0x1A:ld(a, m[de]);
			break;
		case 0x1B:dec16(de);
			break;
		case 0x1C:inc(e);
			break;
		case 0x1D:dec(e);
			break;
		case 0x1E:ld(e, get8(pc+1));
			break;
		case 0x1F:rra();
			break;
		case 0x20:jr(!getf(Z));
			break;
		case 0x21:ld16(hl, get16(pc+1));
			break;
		case 0x22:ld_i(hl);
			hl++;
			break;
		case 0x23:inc16(hl);
			break;
		case 0x24:inc(h);
			break;
		case 0x25:dec(h);
			break;
		case 0x26:ld(h, get8(pc+1));
			break;
		case 0x27:daa();
			break;
		case 0x28:jr(getf(Z));
			break;
		case 0x29:add16(hl);
			break;
		case 0x2A:ld(a, hl);
			(hl)++;
			break;
		case 0x2B:dec16(hl);
			break;
		case 0x2C:inc(l);
			break;
		case 0x2D:dec(l);
			break;
		case 0x2E:ld(l, get8(pc+1));
			break;
		case 0x2F:a = ~a;
			setf(N);
			setf(H);
			break;
		case 0x30:jr(!getf(C));
			break;
		case 0x31:ld16(sp, get16(pc+1));
			break;
		case 0x32:ld_i(hl);
			(hl)--;
			break;
		case 0x33:inc16(sp);
			break;
		case 0x34:inc(m[hl]);
			break;
		case 0x35:dec(m[hl]);
			break;
		case 0x36:ld(m[hl], get8(pc+1));
			break;
		case 0x37:setf(C);
		clrf(N);
		clrf(H);
			break;
		case 0x38:jr(getf(C)); 
			break;
		case 0x39:add16(sp);
			break;
		case 0x3A:ld(a, m[hl]);
			(hl)--;
			break;
		case 0x3B:dec16(sp);
			break;
		case 0x3C:inc(a);
			break;
		case 0x3D:dec(a);
			break;
		case 0x3E:ld(a, get8(pc+1));
			break;
		case 0x3F:ccf();
			break;
		case 0x40:ld(b, b);
			break;
		case 0x41:ld(b, c);
			break;
		case 0x42:ld(b, d);
			break;
		case 0x43:ld(b, e);
			break;
		case 0x44:ld(b, h);
			break;
		case 0x45:ld(b, l);
			break;
		case 0x46:ld(b, m[hl]);
			break;
		case 0x47:ld(b, a);
			break;
		case 0x48:ld(c, b);
			break;
		case 0x49:ld(c, c);
			break;
		case 0x4A:ld(c, d);
			break;
		case 0x4B:ld(c, e);
			break;
		case 0x4C:ld(c, h);
			break;
		case 0x4D:ld(c, l);
			break;
		case 0x4E:ld(c, m[hl]);
			break;
		case 0x4F:ld(c, a);
			break;
		case 0x50:ld(d, b);
			break;
		case 0x51:ld(d, c);
			break;
		case 0x52:ld(d, d);
			break;
		case 0x53:ld(d, e);
			break;
		case 0x54:ld(d, h);
			break;
		case 0x55:ld(d, l);
			break;
		case 0x56:ld(d, m[hl]);
			break;
		case 0x57:ld(d, a);
			break;
		case 0x58:ld(e, b);
			break;
		case 0x59:ld(e, c);
			break;
		case 0x5A:ld(e, d);
			break;
		case 0x5B:ld(e, e);
			break;
		case 0x5C:ld(e, h);
			break;
		case 0x5D:ld(e, l);
			break;
		case 0x5E:ld(e, m[hl]);
			break;
		case 0x5F:ld(e, a);
			break;
		case 0x60:ld(h, b);
			break;
		case 0x61:ld(h, c);
			break;
		case 0x62:ld(h, d);
			break;
		case 0x63:ld(h, e);
			break;
		case 0x64:ld(h, h);
			break;
		case 0x65:ld(h, l);
			break;
		case 0x66:ld(h, m[hl]);
			break;
		case 0x67:ld(h, a);
			break;
		case 0x68:ld(l, b);
			break;
		case 0x69:ld(l, c);
			break;
		case 0x6A:ld(l, d);
			break;
		case 0x6B:ld(l, e);
			break;
		case 0x6C:ld(l, h);
			break;
		case 0x6D:ld(l, l);
			break;
		case 0x6E:ld(l, m[hl]);
			break;
		case 0x6F:ld(l, a);
			break;
		case 0x70:ld(m[hl], b);
			break;
		case 0x71:ld(m[hl], c);
			break;
		case 0x72:ld(m[hl], d);
			break;
		case 0x73:ld(m[hl], e);
			break;
		case 0x74:ld(m[hl], h);
			break;
		case 0x75:ld(m[hl], l);
			break;
		case 0x76: state->halted = true;
			break;
		case 0x77:ld(m[hl], a);
			break;
		case 0x78:ld(a, b);
			break;
		case 0x79:ld(a, c);
			break;
		case 0x7A:ld(a, d);
			break;
		case 0x7B:ld(a, e);
			break;
		case 0x7C:ld(a, h);
			break;
		case 0x7D:ld(a, l);
			break;
		case 0x7E:ld(a, m[hl]);
			break;
		case 0x7F:ld(a, a);
			break;
		case 0x80:add(b);
			break;
		case 0x81:add(c);
			break;
		case 0x82:add(d);
			break;
		case 0x83:add(e);
			break;
		case 0x84:add(h);
			break;
		case 0x85:add(l);
			break;
		case 0x86:add(m[hl]);
			break;
		case 0x87:add(a);
			break;
		case 0x88:adc(b);
			break;
		case 0x89:adc(c);
			break;
		case 0x8A:adc(d);
			break;
		case 0x8B:adc(e);
			break;
		case 0x8C:adc(h);
			break;
		case 0x8D:adc(l);
			break;
		case 0x8E:adc(m[hl]);
			break;
		case 0x8F:adc(a);
			break;
		case 0x90:sub(b);
			break;
		case 0x91:sub(c);
			break;
		case 0x92:sub(d);
			break;
		case 0x93:sub(e);
			break;
		case 0x94:sub(h);
			break;
		case 0x95:sub(l);
			break;
		case 0x96:sub(m[hl]);
			break;
		case 0x97:sub(a);
			break;
		case 0x98:sbc(b);
			break;
		case 0x99:sbc(c);
			break;
		case 0x9A:sbc(d);
			break;
		case 0x9B:sbc(e);
			break;
		case 0x9C:sbc(h);
			break;
		case 0x9D:sbc(l);
			break;
		case 0x9E:sbc(m[hl]);
			break;
		case 0x9F:sbc(a);
			break;
		case 0xA0:_and(b);
			break;
		case 0xA1:_and(c);
			break;
		case 0xA2:_and(d);
			break;
		case 0xA3:_and(e);
			break;
		case 0xA4:_and(h);
			break;
		case 0xA5:_and(l);
			break;
		case 0xA6:_and(m[hl]);
			break;
		case 0xA7:_and(a);
			break;
		case 0xA8:_xor(b);
			break;
		case 0xA9:_xor(c);
			break;
		case 0xAA:_xor(d);
			break;
		case 0xAB:_xor(e);
			break;
		case 0xAC:_xor(h);
			break;
		case 0xAD:_xor(l);
			break;
		case 0xAE:_xor(m[hl]);
			break;
		case 0xAF:_xor(a);
			break;
		case 0xB0:_or(b);
			break;
		case 0xB1:_or(c);
			break;
		case 0xB2:_or(d);
			break;
		case 0xB3:_or(e);
			break;
		case 0xB4:_or(h);
			break;
		case 0xB5:_or(l);
			break;
		case 0xB6:_or(m[hl]);
			break;
		case 0xB7:_or(a);
			break;
		case 0xB8:cp(b);
			break;
		case 0xB9:cp(c);
			break;
		case 0xBA:cp(d);
			break;
		case 0xBB:cp(e);
			break;
		case 0xBC:cp(h);
			break;
		case 0xBD:cp(l);
			break;
		case 0xBE:cp(m[hl]);
			break;
		case 0xBF:cp(a);
			break;
		case 0xC0:ret(!getf(Z));
			break;
		case 0xC1:pop(bc);
			break;
		case 0xC2:jp(!(getf(Z)));
			break;
		case 0xC3:jp(true);
			break;
		case 0xC4:call(!getf(Z));
			break;
		case 0xC5:push(bc);
			break;
		case 0xC6:add(get8(pc+1));
			break;
		case 0xC7:rst(0x00);
			break;
		case 0xC8:ret(getf(Z));
			break;
		case 0xC9:ret(true);
			break;
		case 0xCA:jp(getf(Z));
			break;
		case 0xCB:
			switch (m[pc + 1])
			{
				case 0x00:rlc(b);
					break;
				case 0x01:rlc(c);
					break;
				case 0x02:rlc(d);
					break;
				case 0x03:rlc(e);
					break;
				case 0x04:rlc(h);
					break;
				case 0x05:rlc(l);
					break;
				case 0x06:rlc(m[hl]);
					break;
				case 0x07:rlc(a);
					break;
				case 0x08:rrc(b);
					break;
				case 0x09:rrc(c);
					break;
				case 0x0A:rrc(d);
					break;
				case 0x0B:rrc(e);
					break;
				case 0x0C:rrc(h);
					break;
				case 0x0D:rrc(l);
					break;
				case 0x0E:rrc(m[hl]);
					break;
				case 0x0F:rrc(a);
					break;
				case 0x10:rl(b);
					break;
				case 0x11:rl(c);
					break;
				case 0x12:rl(d);
					break;
				case 0x13:rl(e);
					break;
				case 0x14:rl(h);
					break;
				case 0x15:rl(l);
					break;
				case 0x16:rl(m[hl]);
					break;
				case 0x17:rl(a);
					break;
				case 0x18:rr(b);
					break;
				case 0x19:rr(c);
					break;
				case 0x1A:rr(d);
					break;
				case 0x1B:rr(e);
					break;
				case 0x1C:rr(h);
					break;
				case 0x1D:rr(l);
					break;
				case 0x1E:rr(m[hl]);
					break;
				case 0x1F:rr(a);
					break;
				case 0x20:sla(b);
					break;
				case 0x21:sla(c);
					break;
				case 0x22:sla(d);
					break;
				case 0x23:sla(e);
					break;
				case 0x24:sla(h);
					break;
				case 0x25:sla(l);
					break;
				case 0x26:sla(m[hl]);
					break;
				case 0x27:sla(a);
					break;
				case 0x28:sra(b);
					break;
				case 0x29:sra(c);
					break;
				case 0x2A:sra(d);
					break;
				case 0x2B:sra(e);
					break;
				case 0x2C:sra(h);
					break;
				case 0x2D:sra(l);
					break;
				case 0x2E:sra(m[hl]);
					break;
				case 0x2F:sra(a);
					break;
				case 0x30:swap(b);
					break;
				case 0x31:swap(c);
					break;
				case 0x32:swap(d);
					break;
				case 0x33:swap(e);
					break;
				case 0x34:swap(h);
					break;
				case 0x35:swap(l);
					break;
				case 0x36:swap(m[hl]);
					break;
				case 0x37:swap(a);
					break;
				case 0x38:srl(b);
					break;
				case 0x39:srl(c);
					break;
				case 0x3A:srl(d);
					break;
				case 0x3B:srl(e);
					break;
				case 0x3C:srl(h);
					break;
				case 0x3D:srl(l);
					break;
				case 0x3E:srl(m[hl]);
					break;
				case 0x3F:srl(a);
					break;
				case 0x40:bit(b, 0);
					break;
				case 0x41:bit(c, 0);
					break;
				case 0x42:bit(d, 0);
					break;
				case 0x43:bit(e, 0);
					break;
				case 0x44:bit(h, 0);
					break;
				case 0x45:bit(l, 0);
					break;
				case 0x46:bit(m[hl], 0);
					break;
				case 0x47:bit(a, 0);
					break;
				case 0x48:bit(b, 1);
					break;
				case 0x49:bit(c, 1);
					break;
				case 0x4A:bit(d, 1);
					break;
				case 0x4B:bit(e, 1);
					break;
				case 0x4C:bit(h, 1);
					break;
				case 0x4D:bit(l, 1);
					break;
				case 0x4E:bit(m[hl], 1);
					break;
				case 0x4F:bit(a, 1);
					break;
				case 0x50:bit(b, 2);
					break;
				case 0x51:bit(c, 2);
					break;
				case 0x52:bit(d, 2);
					break;
				case 0x53:bit(e, 2);
					break;
				case 0x54:bit(h, 2);
					break;
				case 0x55:bit(l, 2);
					break;
				case 0x56:bit(m[hl], 2);
					break;
				case 0x57:bit(a, 2);
					break;
				case 0x58:bit(b, 3);
					break;
				case 0x59:bit(c, 3);
					break;
				case 0x5A:bit(d, 3);
					break;
				case 0x5B:bit(e, 3);
					break;
				case 0x5C:bit(h, 3);
					break;
				case 0x5D:bit(l, 3);
					break;
				case 0x5E:bit(m[hl], 3);
					break;
				case 0x5F:bit(a, 3);
					break;
				case 0x60:bit(b, 4);
					break;
				case 0x61:bit(c, 4);
					break;
				case 0x62:bit(d, 4);
					break;
				case 0x63:bit(e, 4);
					break;
				case 0x64:bit(h, 4);
					break;
				case 0x65:bit(l, 4);
					break;
				case 0x66:bit(m[hl], 4);
					break;
				case 0x67:bit(a, 4);
					break;
				case 0x68:bit(b, 5);
					break;
				case 0x69:bit(c, 5);
					break;
				case 0x6A:bit(d, 5);
					break;
				case 0x6B:bit(e, 5);
					break;
				case 0x6C:bit(h, 5);
					break;
				case 0x6D:bit(l, 5);
					break;
				case 0x6E:bit(m[hl], 5);
					break;
				case 0x6F:bit(a, 5);
					break;
				case 0x70:bit(b, 6);
					break;
				case 0x71:bit(c, 6);
					break;
				case 0x72:bit(d, 6);
					break;
				case 0x73:bit(e, 6);
					break;
				case 0x74:bit(h, 6);
					break;
				case 0x75:bit(l, 6);
					break;
				case 0x76:bit(m[hl], 6);
					break;
				case 0x77:bit(a, 6);
					break;
				case 0x78:bit(b, 7);
					break;
				case 0x79:bit(c, 7);
					break;
				case 0x7A:bit(d, 7);
					break;
				case 0x7B:bit(e, 7);
					break;
				case 0x7C:bit(h, 7);
					break;
				case 0x7D:bit(l, 7);
					break;
				case 0x7E:bit(m[hl], 7);
					break;
				case 0x7F:bit(a, 7);
					break;
				case 0x80:res(b, 0);
					break;
				case 0x81:res(c, 0);
					break;
				case 0x82:res(d, 0);
					break;
				case 0x83:res(e, 0);
					break;
				case 0x84:res(h, 0);
					break;
				case 0x85:res(l, 0);
					break;
				case 0x86:res(m[hl], 0);
					break;
				case 0x87:res(a, 0);
					break;
				case 0x88:res(b, 1);
					break;
				case 0x89:res(c, 1);
					break;
				case 0x8A:res(d, 1);
					break;
				case 0x8B:res(e, 1);
					break;
				case 0x8C:res(h, 1);
					break;
				case 0x8D:res(l, 1);
					break;
				case 0x8E:res(m[hl], 1);
					break;
				case 0x8F:res(a, 1);
					break;
				case 0x90:res(b, 2);
					break;
				case 0x91:res(c, 2);
					break;
				case 0x92:res(d, 2);
					break;
				case 0x93:res(e, 2);
					break;
				case 0x94:res(h, 2);
					break;
				case 0x95:res(l, 2);
					break;
				case 0x96:res(m[hl], 2);
					break;
				case 0x97:res(a, 2);
					break;
				case 0x98:res(b, 3);
					break;
				case 0x99:res(c, 3);
					break;
				case 0x9A:res(d, 3);
					break;
				case 0x9B:res(e, 3);
					break;
				case 0x9C:res(h, 3);
					break;
				case 0x9D:res(l, 3);
					break;
				case 0x9E:res(m[hl], 3);
					break;
				case 0x9F:res(a, 3);
					break;
				case 0xA0:res(b, 4);
					break;
				case 0xA1:res(c, 4);
					break;
				case 0xA2:res(d, 4);
					break;
				case 0xA3:res(e, 4);
					break;
				case 0xA4:res(h, 4);
					break;
				case 0xA5:res(l, 4);
					break;
				case 0xA6:res(m[hl], 4);
					break;
				case 0xA7:res(a, 4);
					break;
				case 0xA8:res(b, 5);
					break;
				case 0xA9:res(c, 5);
					break;
				case 0xAA:res(d, 5);
					break;
				case 0xAB:res(e, 5);
					break;
				case 0xAC:res(h, 5);
					break;
				case 0xAD:res(l, 5);
					break;
				case 0xAE:res(m[hl], 5);
					break;
				case 0xAF:res(a, 5);
					break;
				case 0xB0:res(b, 6);
					break;
				case 0xB1:res(c, 6);
					break;
				case 0xB2:res(d, 6);
					break;
				case 0xB3:res(e, 6);
					break;
				case 0xB4:res(h, 6);
					break;
				case 0xB5:res(l, 6);
					break;
				case 0xB6:res(m[hl], 6);
					break;
				case 0xB7:res(a, 6);
					break;
				case 0xB8:res(b, 7);
					break;
				case 0xB9:res(c, 7);
					break;
				case 0xBA:res(d, 7);
					break;
				case 0xBB:res(e, 7);
					break;
				case 0xBC:res(h, 7);
					break;
				case 0xBD:res(l, 7);
					break;
				case 0xBE:res(m[hl], 7);
					break;
				case 0xBF:res(a, 7);
					break;
				case 0xC0:set(b, 0);
					break;
				case 0xC1:set(c, 0);
					break;
				case 0xC2:set(d, 0);
					break;
				case 0xC3:set(e, 0);
					break;
				case 0xC4:set(h, 0);
					break;
				case 0xC5:set(l, 0);
					break;
				case 0xC6:set(m[hl], 0);
					break;
				case 0xC7:set(a, 0);
					break;
				case 0xC8:set(b, 1);
					break;
				case 0xC9:set(c, 1);
					break;
				case 0xCA:set(d, 1);
					break;
				case 0xCB:set(e, 1);
					break;
				case 0xCC:set(h, 1);
					break;
				case 0xCD:set(l, 1);
					break;
				case 0xCE:set(m[hl], 1);
					break;
				case 0xCF:set(a, 1);
					break;
				case 0xD0:set(b, 2);
					break;
				case 0xD1:set(c, 2);
					break;
				case 0xD2:set(d, 2);
					break;
				case 0xD3:set(e, 2);
					break;
				case 0xD4:set(h, 2);
					break;
				case 0xD5:set(l, 2);
					break;
				case 0xD6:set(m[hl], 2);
					break;
				case 0xD7:set(a, 2);
					break;
				case 0xD8:set(b, 3);
					break;
				case 0xD9:set(c, 3);
					break;
				case 0xDA:set(d, 3);
					break;
				case 0xDB:set(e, 3);
					break;
				case 0xDC:set(h, 3);
					break;
				case 0xDD:set(l, 3);
					break;
				case 0xDE:set(m[hl], 3);
					break;
				case 0xDF:set(a, 3);
					break;
				case 0xE0:set(b, 4);
					break;
				case 0xE1:set(c, 4);
					break;
				case 0xE2:set(d, 4);
					break;
				case 0xE3:set(e, 4);
					break;
				case 0xE4:set(h, 4);
					break;
				case 0xE5:set(l, 4);
					break;
				case 0xE6:set(m[hl], 4);
					break;
				case 0xE7:set(a, 4);
					break;
				case 0xE8:set(b, 5);
					break;
				case 0xE9:set(c, 5);
					break;
				case 0xEA:set(d, 5);
					break;
				case 0xEB:set(e, 5);
					break;
				case 0xEC:set(h, 5);
					break;
				case 0xED:set(l, 5);
					break;
				case 0xEE:set(m[hl], 5);
					break;
				case 0xEF:set(a, 5);
					break;
				case 0xF0:set(b, 6);
					break;
				case 0xF1:set(c, 6);
					break;
				case 0xF2:set(d, 6);
					break;
				case 0xF3:set(e, 6);
					break;
				case 0xF4:set(h, 6);
					break;
				case 0xF5:set(l, 6);
					break;
				case 0xF6:set(m[hl], 6);
					break;
				case 0xF7:set(a, 6);
					break;
				case 0xF8:set(b, 7);
					break;
				case 0xF9:set(c, 7);
					break;
				case 0xFA:set(d, 7);
					break;
				case 0xFB:set(e, 7);
					break;
				case 0xFC:set(h, 7);
					break;
				case 0xFD:set(l, 7);
					break;
				case 0xFE:set(m[hl], 7);
					break;
				case 0xFF:set(a, 7);
					break;
				default:spdlog::warn("Unknown CB opcode {0:x}", m[pc + 1]);
					assert(0);
			}
			break;
		case 0xCC:call(getf(Z));
			break;
		case 0xCD:call(true);
			break;
		case 0xCE:adc(get8(pc+1));
			break;
		case 0xCF:rst(0x08);
			break;
		case 0xD0:ret(!getf(C));
			break;
		case 0xD1:pop(de);
			break;
		case 0xD2:jp(!(getf(C)));
			break;
		case 0xD4:call(!getf(C));
			break;
		case 0xD5:push(de);
			break;
		case 0xD6:sub(get8(pc+1));
			break;
		case 0xD7:rst(0x10);
			break;
		case 0xD8:ret(getf(C));
			break;
		case 0xD9:spdlog::info("RETI");
			pc = pop16() - 1;
// Handle end of interrupt
			break;
		case 0xDA:jp(getf(C));
			break;
		case 0xDC:call(getf(C));
			break;
		case 0xDE:sbc(get8(pc+1));
			break;
		case 0xDF:rst(0x18);
			break;
		case 0xE0:ld(m[get8(pc+1) + 0xFF00], a);
			break;
		case 0xE1:pop(hl);
			break;
		case 0xE2:ld(m[c], a);
			break;
		case 0xE5:push(hl);
			break;
		case 0xE6:_and(get8(pc+1));
			break;
		case 0xE7:rst(0x20);
			break;
		case 0xE8:half_carry_check_add16(sp, (signed)get8(pc+1));
			carry_check_add16(sp, (signed)get8(pc+1));
			sp += (signed)get8(pc+1);
			zero_check(sp);
		clrf(N);
			break;
		case 0xE9:jp(m[hl]);
			break;
		case 0xEA:ld(m[get16(pc+1)], a);
			break;
		case 0xEE:_xor(get8(pc+1));
			break;
		case 0xEF:rst(0x28);
			break;
		case 0xF0:ld(a, m[get8(pc+1) + 0xFF00]);
			break;
		case 0xF1:pop(af);
			break;
		case 0xF2:ld(a, m[c]);
			break;
		case 0xF3:state->ime = false;
			break;
		case 0xF5:push(af);
			break;
		case 0xF6:_or(get8(pc+1));
			break;
		case 0xF7:rst(0x30);
			break;
		case 0xF8:half_carry_check_add(sp, (signed)get8(pc+1));
			carry_check_add(sp, (signed)get8(pc+1));
			hl = sp + (signed)get8(pc+1);
		clrf(Z);
		clrf(Z);
			break;
		case 0xF9:ld16(sp, hl);
			break;
		case 0xFA:ld(a, m[get16(pc+1)]);
			break;
		case 0xFB:state->ime = true;
			break;
		case 0xFE:cp(get8(pc+1));
			break;
		case 0xFF:rst(0x38);
			break;
		default:spdlog::error("Unknown opcode {0:x}", m[pc]);
			assert(0);
	}

	pc += ((m[pc] != 0xCB) ? OpsNS::opDefinesTbl[m[pc]].length : OpsNS::cbDefinesTbl[m[pc + 1]].length);
	state->cycles += ((m[pc] != 0xCB) ? OpsNS::opDefinesTbl[m[pc]].cycles[0] : OpsNS::cbDefinesTbl[m[pc + 1]].cycles[0]);
}