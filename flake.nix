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
          # `nix develop`, then e.g. `make keychron/k11_max/ansi_encoder/rgb:squ94wk`.
          # This tree is old QMK 0.14.29, whose in-tree CLI imports appdirs +
          # dotty-dict; pkgs.qmk 1.2.0 dropped appdirs, so inject both into its
          # Python env. python3 covers the Makefile's bare `python3` calls.
          default = pkgs.mkShell {
            packages = with pkgs; [
              (qmk.overridePythonAttrs (o: {
                propagatedBuildInputs = o.propagatedBuildInputs
                  ++ [ python313Packages.appdirs python313Packages.dotty-dict ];
              }))
              python3 gnumake gcc gcc-arm-embedded
            ];
          };
        });
    };
}
