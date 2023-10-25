import { Component, OnInit } from '@angular/core';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

import { SchemeService } from '../scheme.service';
import { SchemesService } from '../../schemes/schemes.service';

@Component({
  selector: 'app-scheme-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.css']
})
export class SettingsComponent implements OnInit {
    is_parent: boolean;

    constructor(
        private schemeService: SchemeService,
        private dialog: MatDialog
    ) { }

    ngOnInit()
    {
        this.is_parent = !this.schemeService.scheme.parent_id;
    }

    open_copy_dialog(): void
    {
        let ref = this.dialog.open(Scheme_Copy_Dialog, { width: '80%' });
        ref.afterClosed().subscribe(res =>
        {
            if (res.scheme_id)
                this.schemeService.copy(res.scheme_id, res.dry_run).subscribe(res =>
                {
                    if (!res) 
                        return;
                    let text = `Result is: ${res.result}\n\n`
                    for (const item_name in res.data)
                    {
                        text += '- ';
                        text += item_name;
                        text += ":\n";

                        const item = res.data[item_name];
                        for (const act_name in item)
                        {
                            text += '  - ';
                            text += act_name;
                            text += ': ';
                            text += item[act_name].toString();
                            text += "\n";
                        }
                    }

                    alert(text);
                });
        });
    }
}

@Component({
    selector: 'app-scheme-copy-dialog',
    template: `
        <h1 mat-dialog-title>Скопировать схему в:</h1>
        <div mat-dialog-content>
            <mat-form-field>
                <mat-label i18n="@@CREATE_SCHEME.SCHEME">Схема</mat-label>
                <mat-select #scheme_list required>
                    <mat-option *ngFor="let c of schemes" [value]="c.id">{{c.title}}</mat-option>
                </mat-select>
            </mat-form-field>
            <mat-checkbox [(ngModel)]="dry_run">Пробный прогон</mat-checkbox>
        </div>
        <div mat-dialog-actions>
            <button mat-button [mat-dialog-close]="{ scheme_id: scheme_list.value, dry_run: dry_run}" [disabled]="!scheme_list.value">Скопировать</button>
            <button mat-button mat-dialog-close cdkFocusInitial>Отмена</button>
        </div>
    `,
    styles: []
})
export class Scheme_Copy_Dialog implements OnInit
{
    schemes: any[] = [];
    dry_run: boolean = true;

    constructor(
        private schemeService: SchemeService,
        private schemesService: SchemesService,
        private dialogRef: MatDialogRef<Scheme_Copy_Dialog>
    ) {}

    ngOnInit(): void
    {
        this.schemesService.get_parent_schemes(this.schemeService.scheme.id).subscribe(schemes => this.schemes = schemes);
    }

    close(): void
    {
        this.dialogRef.close();
    }
}

