#include "stddefs.h"

using namespace OpsNS;

inline void ExeModuleImpl::lookup_and_execute() {
    switch (m[r.pc]) {
        case 0x00:
            break;
        case 0x01:
            ld16(R.bc, GET16);
            break;
        case 0x02:
            ld_i(R.bc);
            break;
        case 0x03:
            inc16(R.bc);
            break;
        case 0x04:
            inc(R.b);
            break;
        case 0x05:
            dec(R.b);
            break;
        case 0x06:
            ld(R.b, GET8);
            break;
        case 0x07:
            rlca();
            break;
        case 0x08:
            mm[GET16] = (_sp & 0xFF);
            break;
        case 0x09:
            add16(_bc);
            break;
        case 0x0A:
            ld(R.a, mm[_bc]);
            break;
        case 0x0B:
            dec16(R.bc);
            break;
        case 0x0C:
            inc(R.c);
            break;
        case 0x0D:
            dec(R.c);
            break;
        case 0x0E:
            ld(R.c, GET8);
            break;
        case 0x0F:
            rrca();
            break;
        case 0x10:
            stopped = true;
            break;
        case 0x11:
            ld16(R.de, GET16);
            break;
        case 0x12:
            ld_i(R.de);
            break;
        case 0x13:
            inc16(R.de);
            break;
        case 0x14:
            inc(R.d);
            break;
        case 0x15:
            dec(R.d);
            break;
        case 0x16:
            ld(R.d, GET8);
            break;
        case 0x17:
            rla();
            break;
        case 0x18:
            jr(true);
            break;
        case 0x19:
            add16(_de);
            break;
        case 0x1A:
            ld(R.a, mm[_de]);
            break;
        case 0x1B:
            dec16(R.de);
            break;
        case 0x1C:
            inc(R.e);
            break;
        case 0x1D:
            dec(R.e);
            break;
        case 0x1E:
            ld(R.e, GET8);
            break;
        case 0x1F:
            rra();
            break;
        case 0x20:
            jr(!(GET(_f, ZBIT)));
            break;
        case 0x21:
            ld16(R.hl, GET16);
            break;
        case 0x22:
            ld_i(R.hl);
            (_hl)++;
            break;
        case 0x23:
            inc16(R.hl);
            break;
        case 0x24:
            inc(R.h);
            break;
        case 0x25:
            dec(R.h);
            break;
        case 0x26:
            ld(R.h, GET8);
            break;
        case 0x27:
            daa();
            break;
        case 0x28:
            jr(GET(_f, ZBIT));
            break;
        case 0x29:
            add16(_hl);
            break;
        case 0x2A:
            ld(R.a, _hl);
            (_hl)++;
            break;
        case 0x2B:
            dec16(R.hl);
            break;
        case 0x2C:
            inc(R.l);
            break;
        case 0x2D:
            dec(R.l);
            break;
        case 0x2E:
            ld(R.l, GET8);
            break;
        case 0x2F:
            _a = ~_a;
            SET(_f, NBIT);
            SET(_f, HBIT);
            break;
        case 0x30:
            jr(!GET(_f, CBIT));
            break;
        case 0x31:
            ld16(R.sp, GET16);
            break;
        case 0x32:
            ld_i(R.hl);
            (_hl)--;
            break;
        case 0x33:
            inc16(R.sp);
            break;
        case 0x34:
            inc(&mm[_hl]);
            break;
        case 0x35:
            dec(&mm[_hl]);
            break;
        case 0x36:
            ld(&mm[_hl], GET8);
            break;
        case 0x37:
            SET(_f, CBIT);
            CLR(_f, NBIT);
            CLR(_f, HBIT);
            break;
        case 0x38:
            jr(GET(_f, CBIT));
            break;
        case 0x39:
            add16(_sp);
            break;
        case 0x3A:
            ld(R.a, mm[_hl]);
            (_hl)--;
            break;
        case 0x3B:
            dec16(R.sp);
            break;
        case 0x3C:
            inc(R.a);
            break;
        case 0x3D:
            dec(R.a);
            break;
        case 0x3E:
            ld(R.a, GET8);
            break;
        case 0x3F:
            ccf();
            break;
        case 0x40:
            ld(R.b, _b);
            break;
        case 0x41:
            ld(R.b, _c);
            break;
        case 0x42:
            ld(R.b, _d);
            break;
        case 0x43:
            ld(R.b, _e);
            break;
        case 0x44:
            ld(R.b, _h);
            break;
        case 0x45:
            ld(R.b, _l);
            break;
        case 0x46:
            ld(R.b, mm[_hl]);
            break;
        case 0x47:
            ld(R.b, _a);
            break;
        case 0x48:
            ld(R.c, _b);
            break;
        case 0x49:
            ld(R.c, _c);
            break;
        case 0x4A:
            ld(R.c, _d);
            break;
        case 0x4B:
            ld(R.c, _e);
            break;
        case 0x4C:
            ld(R.c, _h);
            break;
        case 0x4D:
            ld(R.c, _l);
            break;
        case 0x4E:
            ld(R.c, mm[_hl]);
            break;
        case 0x4F:
            ld(R.c, _a);
            break;
        case 0x50:
            ld(R.d, _b);
            break;
        case 0x51:
            ld(R.d, _c);
            break;
        case 0x52:
            ld(R.d, _d);
            break;
        case 0x53:
            ld(R.d, _e);
            break;
        case 0x54:
            ld(R.d, _h);
            break;
        case 0x55:
            ld(R.d, _l);
            break;
        case 0x56:
            ld(R.d, mm[_hl]);
            break;
        case 0x57:
            ld(R.d, _a);
            break;
        case 0x58:
            ld(R.e, _b);
            break;
        case 0x59:
            ld(R.e, _c);
            break;
        case 0x5A:
            ld(R.e, _d);
            break;
        case 0x5B:
            ld(R.e, _e);
            break;
        case 0x5C:
            ld(R.e, _h);
            break;
        case 0x5D:
            ld(R.e, _l);
            break;
        case 0x5E:
            ld(R.e, mm[_hl]);
            break;
        case 0x5F:
            ld(R.e, _a);
            break;
        case 0x60:
            ld(R.h, _b);
            break;
        case 0x61:
            ld(R.h, _c);
            break;
        case 0x62:
            ld(R.h, _d);
            break;
        case 0x63:
            ld(R.h, _e);
            break;
        case 0x64:
            ld(R.h, _h);
            break;
        case 0x65:
            ld(R.h, _l);
            break;
        case 0x66:
            ld(R.h, mm[_hl]);
            break;
        case 0x67:
            ld(R.h, _a);
            break;
        case 0x68:
            ld(R.l, _b);
            break;
        case 0x69:
            ld(R.l, _c);
            break;
        case 0x6A:
            ld(R.l, _d);
            break;
        case 0x6B:
            ld(R.l, _e);
            break;
        case 0x6C:
            ld(R.l, _h);
            break;
        case 0x6D:
            ld(R.l, _l);
            break;
        case 0x6E:
            ld(R.l, mm[_hl]);
            break;
        case 0x6F:
            ld(R.l, _a);
            break;
        case 0x70:
            ld(&mm[_hl], _b);
            break;
        case 0x71:
            ld(&mm[_hl], _c);
            break;
        case 0x72:
            ld(&mm[_hl], _d);
            break;
        case 0x73:
            ld(&mm[_hl], _e);
            break;
        case 0x74:
            ld(&mm[_hl], _h);
            break;
        case 0x75:
            ld(&mm[_hl], _l);
            break;
        case 0x76:
            halted = true;
            break;
        case 0x77:
            ld(&mm[_hl], _a);
            break;
        case 0x78:
            ld(R.a, _b);
            break;
        case 0x79:
            ld(R.a, _c);
            break;
        case 0x7A:
            ld(R.a, _d);
            break;
        case 0x7B:
            ld(R.a, _e);
            break;
        case 0x7C:
            ld(R.a, _h);
            break;
        case 0x7D:
            ld(R.a, _l);
            break;
        case 0x7E:
            ld(R.a, mm[_hl]);
            break;
        case 0x7F:
            ld(R.a, _a);
            break;
        case 0x80:
            add(_b);
            break;
        case 0x81:
            add(_c);
            break;
        case 0x82:
            add(_d);
            break;
        case 0x83:
            add(_e);
            break;
        case 0x84:
            add(_h);
            break;
        case 0x85:
            add(_l);
            break;
        case 0x86:
            add(mm[_hl]);
            break;
        case 0x87:
            add(_a);
            break;
        case 0x88:
            adc(_b);
            break;
        case 0x89:
            adc(_c);
            break;
        case 0x8A:
            adc(_d);
            break;
        case 0x8B:
            adc(_e);
            break;
        case 0x8C:
            adc(_h);
            break;
        case 0x8D:
            adc(_l);
            break;
        case 0x8E:
            adc(mm[_hl]);
            break;
        case 0x8F:
            adc(_a);
            break;
        case 0x90:
            sub(_b);
            break;
        case 0x91:
            sub(_c);
            break;
        case 0x92:
            sub(_d);
            break;
        case 0x93:
            sub(_e);
            break;
        case 0x94:
            sub(_h);
            break;
        case 0x95:
            sub(_l);
            break;
        case 0x96:
            sub(mm[_hl]);
            break;
        case 0x97:
            sub(_a);
            break;
        case 0x98:
            sbc(_b);
            break;
        case 0x99:
            sbc(_c);
            break;
        case 0x9A:
            sbc(_d);
            break;
        case 0x9B:
            sbc(_e);
            break;
        case 0x9C:
            sbc(_h);
            break;
        case 0x9D:
            sbc(_l);
            break;
        case 0x9E:
            sbc(mm[_hl]);
            break;
        case 0x9F:
            sbc(_a);
            break;
        case 0xA0:
            _and(_b);
            break;
        case 0xA1:
            _and(_c);
            break;
        case 0xA2:
            _and(_d);
            break;
        case 0xA3:
            _and(_e);
            break;
        case 0xA4:
            _and(_h);
            break;
        case 0xA5:
            _and(_l);
            break;
        case 0xA6:
            _and(mm[_hl]);
            break;
        case 0xA7:
            _and(_a);
            break;
        case 0xA8:
            _xor(_b);
            break;
        case 0xA9:
            _xor(_c);
            break;
        case 0xAA:
            _xor(_d);
            break;
        case 0xAB:
            _xor(_e);
            break;
        case 0xAC:
            _xor(_h);
            break;
        case 0xAD:
            _xor(_l);
            break;
        case 0xAE:
            _xor(mm[_hl]);
            break;
        case 0xAF:
            _xor(_a);
            break;
        case 0xB0:
            _or(_b);
            break;
        case 0xB1:
            _or(_c);
            break;
        case 0xB2:
            _or(_d);
            break;
        case 0xB3:
            _or(_e);
            break;
        case 0xB4:
            _or(_h);
            break;
        case 0xB5:
            _or(_l);
            break;
        case 0xB6:
            _or(mm[_hl]);
            break;
        case 0xB7:
            _or(_a);
            break;
        case 0xB8:
            cp(_b);
            break;
        case 0xB9:
            cp(_c);
            break;
        case 0xBA:
            cp(_d);
            break;
        case 0xBB:
            cp(_e);
            break;
        case 0xBC:
            cp(_h);
            break;
        case 0xBD:
            cp(_l);
            break;
        case 0xBE:
            cp(mm[_hl]);
            break;
        case 0xBF:
            cp(_a);
            break;
        case 0xC0:
            ret(!GET(_f, ZBIT));
            break;
        case 0xC1:
            pop(R.bc);
            break;
        case 0xC2:
            jp(!(GET(_f, ZBIT)));
            break;
        case 0xC3:
            jp(true);
            break;
        case 0xC4:
            call(!GET(_f, ZBIT));
            break;
        case 0xC5:
            push(_bc);
            break;
        case 0xC6:
            add(GET8);
            break;
        case 0xC7:
            rst(0x00);
            break;
        case 0xC8:
            ret(GET(_f, ZBIT));
            break;
        case 0xC9:
            ret(true);
            break;
        case 0xCA:
            jp(GET(_f, ZBIT));
            break;
        case 0xCB:
            switch (mm[_pc + 1]) {
                case 0x00:
                    rlc(R.b);
                    break;
                case 0x01:
                    rlc(R.c);
                    break;
                case 0x02:
                    rlc(R.d);
                    break;
                case 0x03:
                    rlc(R.e);
                    break;
                case 0x04:
                    rlc(R.h);
                    break;
                case 0x05:
                    rlc(R.l);
                    break;
                case 0x06:
                    rlc(&mm[_hl]);
                    break;
                case 0x07:
                    rlc(R.a);
                    break;
                case 0x08:
                    rrc(R.b);
                    break;
                case 0x09:
                    rrc(R.c);
                    break;
                case 0x0A:
                    rrc(R.d);
                    break;
                case 0x0B:
                    rrc(R.e);
                    break;
                case 0x0C:
                    rrc(R.h);
                    break;
                case 0x0D:
                    rrc(R.l);
                    break;
                case 0x0E:
                    rrc(&mm[_hl]);
                    break;
                case 0x0F:
                    rrc(R.a);
                    break;
                case 0x10:
                    rl(R.b);
                    break;
                case 0x11:
                    rl(R.c);
                    break;
                case 0x12:
                    rl(R.d);
                    break;
                case 0x13:
                    rl(R.e);
                    break;
                case 0x14:
                    rl(R.h);
                    break;
                case 0x15:
                    rl(R.l);
                    break;
                case 0x16:
                    rl(&mm[_hl]);
                    break;
                case 0x17:
                    rl(R.a);
                    break;
                case 0x18:
                    rr(R.b);
                    break;
                case 0x19:
                    rr(R.c);
                    break;
                case 0x1A:
                    rr(R.d);
                    break;
                case 0x1B:
                    rr(R.e);
                    break;
                case 0x1C:
                    rr(R.h);
                    break;
                case 0x1D:
                    rr(R.l);
                    break;
                case 0x1E:
                    rr(&mm[_hl]);
                    break;
                case 0x1F:
                    rr(R.a);
                    break;
                case 0x20:
                    sla(R.b);
                    break;
                case 0x21:
                    sla(R.c);
                    break;
                case 0x22:
                    sla(R.d);
                    break;
                case 0x23:
                    sla(R.e);
                    break;
                case 0x24:
                    sla(R.h);
                    break;
                case 0x25:
                    sla(R.l);
                    break;
                case 0x26:
                    sla(&mm[_hl]);
                    break;
                case 0x27:
                    sla(R.a);
                    break;
                case 0x28:
                    sra(R.b);
                    break;
                case 0x29:
                    sra(R.c);
                    break;
                case 0x2A:
                    sra(R.d);
                    break;
                case 0x2B:
                    sra(R.e);
                    break;
                case 0x2C:
                    sra(R.h);
                    break;
                case 0x2D:
                    sra(R.l);
                    break;
                case 0x2E:
                    sra(&mm[_hl]);
                    break;
                case 0x2F:
                    sra(R.a);
                    break;
                case 0x30:
                    swap(R.b);
                    break;
                case 0x31:
                    swap(R.c);
                    break;
                case 0x32:
                    swap(R.d);
                    break;
                case 0x33:
                    swap(R.e);
                    break;
                case 0x34:
                    swap(R.h);
                    break;
                case 0x35:
                    swap(R.l);
                    break;
                case 0x36:
                    swap(&mm[_hl]);
                    break;
                case 0x37:
                    swap(R.a);
                    break;
                case 0x38:
                    srl(R.b);
                    break;
                case 0x39:
                    srl(R.c);
                    break;
                case 0x3A:
                    srl(R.d);
                    break;
                case 0x3B:
                    srl(R.e);
                    break;
                case 0x3C:
                    srl(R.h);
                    break;
                case 0x3D:
                    srl(R.l);
                    break;
                case 0x3E:
                    srl(&mm[_hl]);
                    break;
                case 0x3F:
                    srl(R.a);
                    break;
                case 0x40:
                    bit(_b, 0);
                    break;
                case 0x41:
                    bit(_c, 0);
                    break;
                case 0x42:
                    bit(_d, 0);
                    break;
                case 0x43:
                    bit(_e, 0);
                    break;
                case 0x44:
                    bit(_h, 0);
                    break;
                case 0x45:
                    bit(_l, 0);
                    break;
                case 0x46:
                    bit(mm[_hl], 0);
                    break;
                case 0x47:
                    bit(_a, 0);
                    break;
                case 0x48:
                    bit(_b, 1);
                    break;
                case 0x49:
                    bit(_c, 1);
                    break;
                case 0x4A:
                    bit(_d, 1);
                    break;
                case 0x4B:
                    bit(_e, 1);
                    break;
                case 0x4C:
                    bit(_h, 1);
                    break;
                case 0x4D:
                    bit(_l, 1);
                    break;
                case 0x4E:
                    bit(mm[_hl], 1);
                    break;
                case 0x4F:
                    bit(_a, 1);
                    break;
                case 0x50:
                    bit(_b, 2);
                    break;
                case 0x51:
                    bit(_c, 2);
                    break;
                case 0x52:
                    bit(_d, 2);
                    break;
                case 0x53:
                    bit(_e, 2);
                    break;
                case 0x54:
                    bit(_h, 2);
                    break;
                case 0x55:
                    bit(_l, 2);
                    break;
                case 0x56:
                    bit(mm[_hl], 2);
                    break;
                case 0x57:
                    bit(_a, 2);
                    break;
                case 0x58:
                    bit(_b, 3);
                    break;
                case 0x59:
                    bit(_c, 3);
                    break;
                case 0x5A:
                    bit(_d, 3);
                    break;
                case 0x5B:
                    bit(_e, 3);
                    break;
                case 0x5C:
                    bit(_h, 3);
                    break;
                case 0x5D:
                    bit(_l, 3);
                    break;
                case 0x5E:
                    bit(mm[_hl], 3);
                    break;
                case 0x5F:
                    bit(_a, 3);
                    break;
                case 0x60:
                    bit(_b, 4);
                    break;
                case 0x61:
                    bit(_c, 4);
                    break;
                case 0x62:
                    bit(_d, 4);
                    break;
                case 0x63:
                    bit(_e, 4);
                    break;
                case 0x64:
                    bit(_h, 4);
                    break;
                case 0x65:
                    bit(_l, 4);
                    break;
                case 0x66:
                    bit(mm[_hl], 4);
                    break;
                case 0x67:
                    bit(_a, 4);
                    break;
                case 0x68:
                    bit(_b, 5);
                    break;
                case 0x69:
                    bit(_c, 5);
                    break;
                case 0x6A:
                    bit(_d, 5);
                    break;
                case 0x6B:
                    bit(_e, 5);
                    break;
                case 0x6C:
                    bit(_h, 5);
                    break;
                case 0x6D:
                    bit(_l, 5);
                    break;
                case 0x6E:
                    bit(mm[_hl], 5);
                    break;
                case 0x6F:
                    bit(_a, 5);
                    break;
                case 0x70:
                    bit(_b, 6);
                    break;
                case 0x71:
                    bit(_c, 6);
                    break;
                case 0x72:
                    bit(_d, 6);
                    break;
                case 0x73:
                    bit(_e, 6);
                    break;
                case 0x74:
                    bit(_h, 6);
                    break;
                case 0x75:
                    bit(_l, 6);
                    break;
                case 0x76:
                    bit(mm[_hl], 6);
                    break;
                case 0x77:
                    bit(_a, 6);
                    break;
                case 0x78:
                    bit(_b, 7);
                    break;
                case 0x79:
                    bit(_c, 7);
                    break;
                case 0x7A:
                    bit(_d, 7);
                    break;
                case 0x7B:
                    bit(_e, 7);
                    break;
                case 0x7C:
                    bit(_h, 7);
                    break;
                case 0x7D:
                    bit(_l, 7);
                    break;
                case 0x7E:
                    bit(mm[_hl], 7);
                    break;
                case 0x7F:
                    bit(_a, 7);
                    break;
                case 0x80:
                    res(R.b, 0);
                    break;
                case 0x81:
                    res(R.c, 0);
                    break;
                case 0x82:
                    res(R.d, 0);
                    break;
                case 0x83:
                    res(R.e, 0);
                    break;
                case 0x84:
                    res(R.h, 0);
                    break;
                case 0x85:
                    res(R.l, 0);
                    break;
                case 0x86:
                    res(&mm[_hl], 0);
                    break;
                case 0x87:
                    res(R.a, 0);
                    break;
                case 0x88:
                    res(R.b, 1);
                    break;
                case 0x89:
                    res(R.c, 1);
                    break;
                case 0x8A:
                    res(R.d, 1);
                    break;
                case 0x8B:
                    res(R.e, 1);
                    break;
                case 0x8C:
                    res(R.h, 1);
                    break;
                case 0x8D:
                    res(R.l, 1);
                    break;
                case 0x8E:
                    res(&mm[_hl], 1);
                    break;
                case 0x8F:
                    res(R.a, 1);
                    break;
                case 0x90:
                    res(R.b, 2);
                    break;
                case 0x91:
                    res(R.c, 2);
                    break;
                case 0x92:
                    res(R.d, 2);
                    break;
                case 0x93:
                    res(R.e, 2);
                    break;
                case 0x94:
                    res(R.h, 2);
                    break;
                case 0x95:
                    res(R.l, 2);
                    break;
                case 0x96:
                    res(&mm[_hl], 2);
                    break;
                case 0x97:
                    res(R.a, 2);
                    break;
                case 0x98:
                    res(R.b, 3);
                    break;
                case 0x99:
                    res(R.c, 3);
                    break;
                case 0x9A:
                    res(R.d, 3);
                    break;
                case 0x9B:
                    res(R.e, 3);
                    break;
                case 0x9C:
                    res(R.h, 3);
                    break;
                case 0x9D:
                    res(R.l, 3);
                    break;
                case 0x9E:
                    res(&mm[_hl], 3);
                    break;
                case 0x9F:
                    res(R.a, 3);
                    break;
                case 0xA0:
                    res(R.b, 4);
                    break;
                case 0xA1:
                    res(R.c, 4);
                    break;
                case 0xA2:
                    res(R.d, 4);
                    break;
                case 0xA3:
                    res(R.e, 4);
                    break;
                case 0xA4:
                    res(R.h, 4);
                    break;
                case 0xA5:
                    res(R.l, 4);
                    break;
                case 0xA6:
                    res(&mm[_hl], 4);
                    break;
                case 0xA7:
                    res(R.a, 4);
                    break;
                case 0xA8:
                    res(R.b, 5);
                    break;
                case 0xA9:
                    res(R.c, 5);
                    break;
                case 0xAA:
                    res(R.d, 5);
                    break;
                case 0xAB:
                    res(R.e, 5);
                    break;
                case 0xAC:
                    res(R.h, 5);
                    break;
                case 0xAD:
                    res(R.l, 5);
                    break;
                case 0xAE:
                    res(&mm[_hl], 5);
                    break;
                case 0xAF:
                    res(R.a, 5);
                    break;
                case 0xB0:
                    res(R.b, 6);
                    break;
                case 0xB1:
                    res(R.c, 6);
                    break;
                case 0xB2:
                    res(R.d, 6);
                    break;
                case 0xB3:
                    res(R.e, 6);
                    break;
                case 0xB4:
                    res(R.h, 6);
                    break;
                case 0xB5:
                    res(R.l, 6);
                    break;
                case 0xB6:
                    res(&mm[_hl], 6);
                    break;
                case 0xB7:
                    res(R.a, 6);
                    break;
                case 0xB8:
                    res(R.b, 7);
                    break;
                case 0xB9:
                    res(R.c, 7);
                    break;
                case 0xBA:
                    res(R.d, 7);
                    break;
                case 0xBB:
                    res(R.e, 7);
                    break;
                case 0xBC:
                    res(R.h, 7);
                    break;
                case 0xBD:
                    res(R.l, 7);
                    break;
                case 0xBE:
                    res(&mm[_hl], 7);
                    break;
                case 0xBF:
                    res(R.a, 7);
                    break;
                case 0xC0:
                    set(R.b, 0);
                    break;
                case 0xC1:
                    set(R.c, 0);
                    break;
                case 0xC2:
                    set(R.d, 0);
                    break;
                case 0xC3:
                    set(R.e, 0);
                    break;
                case 0xC4:
                    set(R.h, 0);
                    break;
                case 0xC5:
                    set(R.l, 0);
                    break;
                case 0xC6:
                    set(&mm[_hl], 0);
                    break;
                case 0xC7:
                    set(R.a, 0);
                    break;
                case 0xC8:
                    set(R.b, 1);
                    break;
                case 0xC9:
                    set(R.c, 1);
                    break;
                case 0xCA:
                    set(R.d, 1);
                    break;
                case 0xCB:
                    set(R.e, 1);
                    break;
                case 0xCC:
                    set(R.h, 1);
                    break;
                case 0xCD:
                    set(R.l, 1);
                    break;
                case 0xCE:
                    set(&mm[_hl], 1);
                    break;
                case 0xCF:
                    set(R.a, 1);
                    break;
                case 0xD0:
                    set(R.b, 2);
                    break;
                case 0xD1:
                    set(R.c, 2);
                    break;
                case 0xD2:
                    set(R.d, 2);
                    break;
                case 0xD3:
                    set(R.e, 2);
                    break;
                case 0xD4:
                    set(R.h, 2);
                    break;
                case 0xD5:
                    set(R.l, 2);
                    break;
                case 0xD6:
                    set(&mm[_hl], 2);
                    break;
                case 0xD7:
                    set(R.a, 2);
                    break;
                case 0xD8:
                    set(R.b, 3);
                    break;
                case 0xD9:
                    set(R.c, 3);
                    break;
                case 0xDA:
                    set(R.d, 3);
                    break;
                case 0xDB:
                    set(R.e, 3);
                    break;
                case 0xDC:
                    set(R.h, 3);
                    break;
                case 0xDD:
                    set(R.l, 3);
                    break;
                case 0xDE:
                    set(&mm[_hl], 3);
                    break;
                case 0xDF:
                    set(R.a, 3);
                    break;
                case 0xE0:
                    set(R.b, 4);
                    break;
                case 0xE1:
                    set(R.c, 4);
                    break;
                case 0xE2:
                    set(R.d, 4);
                    break;
                case 0xE3:
                    set(R.e, 4);
                    break;
                case 0xE4:
                    set(R.h, 4);
                    break;
                case 0xE5:
                    set(R.l, 4);
                    break;
                case 0xE6:
                    set(&mm[_hl], 4);
                    break;
                case 0xE7:
                    set(R.a, 4);
                    break;
                case 0xE8:
                    set(R.b, 5);
                    break;
                case 0xE9:
                    set(R.c, 5);
                    break;
                case 0xEA:
                    set(R.d, 5);
                    break;
                case 0xEB:
                    set(R.e, 5);
                    break;
                case 0xEC:
                    set(R.h, 5);
                    break;
                case 0xED:
                    set(R.l, 5);
                    break;
                case 0xEE:
                    set(&mm[_hl], 5);
                    break;
                case 0xEF:
                    set(R.a, 5);
                    break;
                case 0xF0:
                    set(R.b, 6);
                    break;
                case 0xF1:
                    set(R.c, 6);
                    break;
                case 0xF2:
                    set(R.d, 6);
                    break;
                case 0xF3:
                    set(R.e, 6);
                    break;
                case 0xF4:
                    set(R.h, 6);
                    break;
                case 0xF5:
                    set(R.l, 6);
                    break;
                case 0xF6:
                    set(&mm[_hl], 6);
                    break;
                case 0xF7:
                    set(R.a, 6);
                    break;
                case 0xF8:
                    set(R.b, 7);
                    break;
                case 0xF9:
                    set(R.c, 7);
                    break;
                case 0xFA:
                    set(R.d, 7);
                    break;
                case 0xFB:
                    set(R.e, 7);
                    break;
                case 0xFC:
                    set(R.h, 7);
                    break;
                case 0xFD:
                    set(R.l, 7);
                    break;
                case 0xFE:
                    set(&mm[_hl], 7);
                    break;
                case 0xFF:
                    set(R.a, 7);
                    break;
                default:
                    spdlog::warn("Unknown CB opcode {0:x}", mm[_pc + 1]);
                    assert(0);
            }
            break;
        case 0xCC:
            call(GET(_f, ZBIT));
            break;
        case 0xCD:
            call(true);
            break;
        case 0xCE:
            adc(GET8);
            break;
        case 0xCF:
            rst(0x08);
            break;
        case 0xD0:
            ret(!GET(_f, CBIT));
            break;
        case 0xD1:
            pop(R.de);
            break;
        case 0xD2:
            jp(!(GET(_f, CBIT)));
            break;
        case 0xD4:
            call(!GET(_f, CBIT));
            break;
        case 0xD5:
            push(_de);
            break;
        case 0xD6:
            sub(GET8);
            break;
        case 0xD7:
            rst(0x10);
            break;
        case 0xD8:
            ret(GET(_f, CBIT));
            break;
        case 0xD9:
            spdlog::info("RETI");
            _pc = POP16 - 1;
            // Handle end of interrupt
            break;
        case 0xDA:
            jp(GET(_f, CBIT));
            break;
        case 0xDC:
            call(GET(_f, CBIT));
            break;
        case 0xDE:
            sbc(GET8);
            break;
        case 0xDF:
            rst(0x18);
            break;
        case 0xE0:
            ld(&mm[GET8 + 0xFF00], _a);
            break;
        case 0xE1:
            pop(R.hl);
            break;
        case 0xE2:
            ld(&mm[_c], _a);
            break;
        case 0xE5:
            push(_hl);
            break;
        case 0xE6:
            _and(GET8);
            break;
        case 0xE7:
            rst(0x20);
            break;
        case 0xE8:
            HCHKADD16(_sp, (signed) GET8);
            CYCHKADD16(_sp, (signed) GET8);
            _sp += (signed) GET8;
            ZCHK(_sp);
            CLR(_f, NBIT);
            break;
        case 0xE9:
            jp(mm[_hl]);
            break;
        case 0xEA:
            ld(&mm[GET16], _a);
            break;
        case 0xEE:
            _xor(GET8);
            break;
        case 0xEF:
            rst(0x28);
            break;
        case 0xF0:
            ld(R.a, mm[GET8 + 0xFF00]);
            break;
        case 0xF1:
            pop(R.af);
            break;
        case 0xF2:
            ld(R.a, mm[_c]);
            break;
        case 0xF3:
            ime = false;
            break;
        case 0xF5:
            push(_af);
            break;
        case 0xF6:
            _or(GET8);
            break;
        case 0xF7:
            rst(0x30);
            break;
        case 0xF8:
            HCHKADD(_sp, (signed) GET8);
            CYCHKADD(_sp, (signed) GET8);
            _hl = _sp + (signed) GET8;
            CLR(_f, ZBIT);
            CLR(_f, NBIT);
            break;
        case 0xF9:
            ld16(R.sp, _hl);
            break;
        case 0xFA:
            ld(R.a, mm[GET16]);
            break;
        case 0xFB:
            ime = true;
            break;
        case 0xFE:
            cp(GET8);
            break;
        case 0xFF:
            rst(0x38);
            break;
        default:
            spdlog::error("Unknown opcode {0:x}", mm[_pc]);
            assert(0);
    }
}