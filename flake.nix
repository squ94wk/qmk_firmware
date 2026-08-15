{
  description = "Dev shell for building this QMK firmware fork";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in {
      devShells = forAllSystems (system:
        let pkgs = nixpkgs.legacyPackages.${system};
        in {
          # `nix develop`, then e.g. `make magic_mirror/v1/left:squ94wk:uf2`.
          # qmk carries its own Python; python3 is here for the Makefile's
          # bare `python3` calls. gcc-arm-embedded is the RP2040 (ARM) toolchain.
          default = pkgs.mkShell {
            packages = with pkgs; [ qmk python3 gnumake gcc gcc-arm-embedded ];
          };
        });
    };
}
