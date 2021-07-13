//
// Created by Jon on 7/11/2021.
//

#pragma once
#include <array>
#include <functional>

namespace ModernBoy {

class Registers {
 public:
  std::reference_wrapper<uint16_t>  af{std::reference_wrapper<uint16_t>(&reg_[0])},
                                    bc{std::reference_wrapper<uint16_t>(&reg_[2])},
                                    de{std::reference_wrapper<uint16_t>(&reg_[4])},
                                    hl{std::reference_wrapper<uint16_t>(&reg_[6])},
                                    sp{std::reference_wrapper<uint16_t>(&reg_[8])},
                                    pc{std::reference_wrapper<uint16_t>(&reg_[10])};
  std::reference_wrapper<uint8_t>   a{std::reference_wrapper<uint8_t>(&reg_[1])},f{std::reference_wrapper<uint8_t>(&reg_[0])},
                                    b{std::reference_wrapper<uint8_t>(&reg_[3])},c{std::reference_wrapper<uint8_t>(&reg_[2])},
                                    d{std::reference_wrapper<uint8_t>(&reg_[5])},d{std::reference_wrapper<uint8_t>(&reg_[4])},
                                    h{std::reference_wrapper<uint8_t>(&reg_[7])},l{std::reference_wrapper<uint8_t>(&reg_[6])};
 private:
  uint8_t reg_[6]{0};
};

}

