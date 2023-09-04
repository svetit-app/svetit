import {Component, Input, OnChanges, OnInit, SimpleChanges} from '@angular/core';
import {SchemesService} from '../../schemes/schemes.service';
import {Group_User_Roles, Scheme, UserHeader, UserHeaderWithRole} from '../../user';
import {User} from '../../user/model';
import {UntypedFormBuilder, UntypedFormGroup, ValidatorFn, Validators} from '@angular/forms';
import {DropdownSettings} from 'angular2-multiselect-dropdown/lib/multiselect.interface';
import {TranslateService} from '@ngx-translate/core';

@Component({
    selector: 'app-users-and-schemes',
    templateUrl: './users-and-schemes.component.html',
    styleUrls: ['./users-and-schemes.component.css']
})
export class UsersAndSchemesComponent implements OnInit, OnChanges {
    readonly Group_User_Roles = Group_User_Roles;
    readonly displayedUsersColumns = ['name', 'username', 'role', 'control'];

    @Input() id: number;

    groupUsers: UserHeaderWithRole[];
    groupSchemes: Pick<Scheme, 'id' | 'name' | 'title'>[];

    userAddFg: UntypedFormGroup;
    schemeAddFg: UntypedFormGroup;
    invitingUser = false;

    schemes: Pick<Scheme, 'id' | 'name' | 'title'>[] = [];
    users: UserHeader[] = [];

    usersLoading: boolean = false;
    schemesLoading: boolean = false;

    private readonly defaultMultiselectSettings = {
        singleSelection: true,
        enableCheckAll: false,
        enableSearchFilter: true,
        lazyLoading: true,
        classes: 'select',
        labelKey: 'label',
        primaryKey: 'id',
    } as DropdownSettings;
    readonly usersMultiselectSettings = {
        ...this.defaultMultiselectSettings,
        searchBy: ['firstname', 'lastname', 'username'],
        text: 'User', // TODO: this.translate.instant('@@SCHEME_GROUPS.USER')
    } as DropdownSettings;
    readonly schemesMultiselectSettings = {
        ...this.defaultMultiselectSettings,
        searchBy: ['name', 'title'],
        text: 'Scheme', // TODO: this.translate.instant('@@SCHEME_GROUPS.SCHEME'),
    } as DropdownSettings;

    constructor(
        private schemesService: SchemesService,
        private translate: TranslateService,
        fb: UntypedFormBuilder,
    ) {
        this.userAddFg = fb.group({
            email: [null, []],
            user: [null, [Validators.required]],
            role: [null, [Validators.required]],
        }, {
            validators: [this.emailOrUserValidator()],
        });

        this.schemeAddFg = fb.group({
            scheme: [null, [Validators.required]],
        });
    }

    ngOnInit(): void {
    }

    ngOnChanges(changes: SimpleChanges) {
        if (changes.id && changes.id.currentValue) {
            this.getData();
        }
    }

    fetchMoreUsers(event) {
        if (this.usersLoading === false && event.endIndex === this.users.length - 1) {
            this.usersLoading = true;
            this.schemesService.getUserHeaders(10, Math.ceil(this.users.length / 10))
                .subscribe((users) => {
                    this.users = this.users.concat(
                        users.map(user => ({
                            ...user,
                            label: `${user.firstname} ${user.lastname} (${user.username})`,
                        })),
                    );
                    this.usersLoading = users.length ? false : null;
                });
        }
    }

    fetchMoreSchemes(event) {
        if (this.schemesLoading === false && event.endIndex === this.schemes.length - 1) {
            this.schemesLoading = true;
            this.schemesService.getSchemes(10, Math.ceil(this.schemes.length / 10))
                .subscribe((schemes) => {
                    this.schemes = this.schemes.concat(
                        schemes.results.map(scheme => ({
                            ...scheme,
                            label: scheme.title || scheme.name,
                        })),
                    );
                    this.schemesLoading = schemes.results.length ? false : null;
                });
        }
    }

    private getData() {
        this.schemesService.getSchemeGroupUsers(this.id).subscribe(users => this.groupUsers = users);
        this.schemesService.getSchemeGroupSchemes(this.id).subscribe(groupSchemes => this.groupSchemes = groupSchemes);
    }

    schemeAddFormSubmit() {
        if (this.schemeAddFg.invalid) return;

        const schemes = this.schemeAddFg.value.scheme;
        if (schemes && schemes.length) {
            this.schemesService.addSchemeToSchemeGroup(this.id, schemes[0].id)
                .subscribe(() => this.getData());
            this.schemeAddFg.reset();
        }
    }

    userAddFormSubmit() {
        if (this.userAddFg.invalid) return;

        const users = this.userAddFg.value.user;
        if (users && users.length) {
            this.schemesService.addUserToSchemeGroup(this.id, users[0].id, this.userAddFg.value.role)
                .subscribe(() => this.getData());
        }
        else if (this.userAddFg.value.email) {
            console.warn("Not implemented");
        }

        this.userAddFg.reset();
    }

    removeUserFromGroup(user: User & { role: Group_User_Roles }) {
        this.schemesService.removeUserFromSchemeGroup(this.id, user.id)
            .subscribe(() => this.getData());
    }

    removeSchemeFromGroup(scheme: Scheme) {
        this.schemesService.removeSchemeFromSchemeGroup(this.id, scheme.id)
            .subscribe(() => this.getData());
    }

    emailOrUserValidator(): ValidatorFn {
        return (fg: UntypedFormGroup) => {
            const ctrl = this.invitingUser ? fg.controls.email : fg.controls.user;

            if (ctrl.untouched || ctrl.invalid) {
                return { 'userRequired': true };
            }

            return null;
        };
    }

    toggleInvitingUser() {
        this.invitingUser = !this.invitingUser;

        const { user, email } = this.userAddFg.controls;

        if (this.invitingUser) {
            user.clearValidators();
            user.setValue(null);
            email.setValidators([Validators.required, Validators.email]);
        } else {
            email.clearValidators();
            email.setValue(null);
            user.setValidators([Validators.required]);
        }
    }
}
