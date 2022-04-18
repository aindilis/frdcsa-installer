FROM debian/latest

COPY "install.sh" "~/install.sh"
COPY "helper" "~/helper"

CMD ["~/install.sh"]
