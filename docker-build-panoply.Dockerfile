FROM debian/latest

COPY "frdcsa-panoply-installer-20220403.sh" "~/frdcsa-panoply-installer-20220403.sh"
COPY "helper" "~/helper"

CMD ["~/frdcsa-panoply-installer-20220403.sh"]
